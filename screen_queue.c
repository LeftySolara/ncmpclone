/******************************************************************************
 * screen_queue.c : main window for the queue screen
 * ****************************************************************************
 * Copyright (C) 2017 Jalen Adams
 *
 * Authors: Jalen Adams <leftysolara@gmail.com>
 *
 * This file is part of ncmpclone.
 *
 * ncmpclone is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ncmpclone is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ncmpclone.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "screen_queue.h"
#include <stdlib.h>
#include <string.h>


struct screen_queue *screen_queue_init()
{
    struct screen_queue *screen_queue = malloc(sizeof(*screen_queue));
    screen_queue->list = list_init();
    screen_queue->queue_version = mpd_status_get_queue_version(mpd_info->status);
    screen_queue->current_song_pos = -1;
    screen_queue->last_state = MPD_STATE_UNKNOWN;

    return screen_queue;
}

void screen_queue_free(struct screen_queue *screen_queue)
{
    list_free(screen_queue->list);
    screen_queue->list = NULL;
    free(screen_queue);
}

void screen_queue_populate_list(struct screen_queue *screen_queue)
{
    if (!mpd_info->connection)
        return;

    struct mpd_song *song;
    bool bold;
    mpd_send_list_queue_meta(mpd_info->connection);

    while ((song = mpd_recv_song(mpd_info->connection))) {

        bold = ((mpd_song_get_pos(song) == mpd_status_get_song_pos(mpd_info->status))
               && mpd_status_get_state(mpd_info->status) != MPD_STATE_STOP) ?
               true : false;

        list_append_item(screen_queue->list,
                         create_track_label(song),
                         create_duration_label(song),
                         bold);
    }
    screen_queue->queue_version = mpd_status_get_queue_version(mpd_info->status);
}

void screen_queue_update(struct screen_queue *screen_queue)
{
    /* If the queue or current song haven't changed, don't send queries */
    if (screen_queue->queue_version == mpd_status_get_queue_version(mpd_info->status)
            && screen_queue->current_song_pos == mpd_status_get_song_pos(mpd_info->status)
            && screen_queue->last_state == mpd_status_get_state(mpd_info->status))
        return;

    /* Store page scroll and cursor positions */
    int idx_top_visible = 0;
    struct list_item *current = screen_queue->list->head;
    while (current && current->next && current != screen_queue->list->top_visible) {
        current = current->next;
        ++idx_top_visible;
    }
    int idx_selected = screen_queue->list->selected_index;
    screen_queue->current_song_pos = mpd_status_get_song_pos(mpd_info->status);

    screen_queue_clear(screen_queue);
    screen_queue_populate_list(screen_queue);

    /* Restore page scroll position */
    int i = 0;
    current = screen_queue->list->head;
    while (current && current->next && i++ < idx_top_visible)
        current = current->next;
    screen_queue->list->top_visible = current;

    /* Restore cursor position */
    for (int i = 0; i < idx_selected; ++i)
        screen_queue_move_cursor(screen_queue, DOWN);

    screen_queue->list->selected_index = idx_selected;
    screen_queue->last_state = mpd_status_get_state(mpd_info->status);
    screen_queue_draw(screen_queue);
}

/* Create a song label of the format "artist - title" */
char *create_track_label(struct mpd_song *song)
{
    const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
    const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
    char *buffer = malloc(strlen(artist) + strlen(title) + 4);

    if (buffer == NULL) {
        printf("Cannot create track label. No memory available.");
    }
    else {
        strcpy(buffer, artist);
        strcat(buffer, " - ");
        strcat(buffer, title);
    }

    return buffer;
}

/* Create a human-readable string representing a song's duration */
char *create_duration_label(struct mpd_song *song)
{
    const int buf_size = 7;
    char *buffer = calloc(buf_size, sizeof(char));

    if (buffer == NULL)
        printf("Cannot create duration string: not enough memory available.");
    else {
        int minutes = mpd_song_get_duration(song) / 60;
        int seconds = mpd_song_get_duration(song) % 60;
        snprintf(buffer, buf_size, "%d:%02d", minutes, seconds);
    }

    return buffer;
}

void screen_queue_cmd(command_t cmd, struct screen_queue *screen)
{
    switch (cmd) {
        case CMD_LIST_MOVE_UP:
            screen_queue_move_cursor(screen, UP);
            break;
        case CMD_LIST_MOVE_DOWN:
            screen_queue_move_cursor(screen, DOWN);
            break;
        case CMD_LIST_MOVE_TOP:
            screen_queue_move_pos(screen, TOP);
            break;
        case CMD_LIST_MOVE_MID:
            screen_queue_move_pos(screen, MID);
            break;
        case CMD_LIST_MOVE_BOTTOM:
            screen_queue_move_pos(screen, BOT);
            break;
        case CMD_LIST_PAGE_UP:
            screen_queue_scroll_page(screen, UP);
            break;
        case CMD_LIST_PAGE_DOWN:
            screen_queue_scroll_page(screen, DOWN);
            break;
        case CMD_PLAY:
            mpd_run_play_pos(mpd_info->connection, screen->list->selected_index);
            break;
        case CMD_CLEAR_QUEUE:
            mpd_run_clear(mpd_info->connection);
            break;
    }
}
