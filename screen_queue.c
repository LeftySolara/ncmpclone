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
    if (!mpd_info->queue_head)
        return;

    struct queue_song *queue_song = mpd_info->queue_head;
    while (queue_song) {
        list_append_item(screen_queue->list,
                         create_track_label(queue_song->song),
                         create_duration_label(queue_song->song));
        queue_song = queue_song->next;
    }
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
            list_move_cursor(screen->list, UP);
            break;
        case CMD_LIST_MOVE_DOWN:
            list_move_cursor(screen->list, DOWN);
            break;
        case CMD_LIST_PAGE_UP:
            list_scroll_page(screen->list, UP);
            break;
        case CMD_LIST_PAGE_DOWN:
            list_scroll_page(screen->list, DOWN);
            break;
        case CMD_PLAY:
            mpd_run_play_pos(mpd_info->connection, screen->list->selected_index);
            break;
    }
}
