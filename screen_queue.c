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

struct queue_row *queue_row_init(struct mpd_song *song)
{
    struct queue_row *row = malloc(sizeof(*row));
    row->song = song;
    row->song_label = create_track_label(song);
    row->duration_label = create_duration_label(song);
    row->is_selected = false;
    row->next = NULL;
    row->prev = NULL;

    return row;
}

void queue_row_free(struct queue_row *row)
{
    row->next = NULL;
    row->prev = NULL;

    if (row->song)
        mpd_song_free(row->song);
    if (row->song_label)
        free(row->song_label);
    if (row->duration_label)
        free(row->duration_label);

    free(row);
}

/*****************************************************************************/

struct screen_queue *screen_queue_init()
{
    struct screen_queue *window = malloc(sizeof(*window));

    /* The window starts just under the title bar and uses the full width
     * of the screen */
    window->win = newwin(LINES - 4, COLS, 2, 0);
    window->head = NULL;
    window->selected = NULL;
    window->top_visible = window->head;
    window->bottom_visible = window->head;
    window->max_visible = LINES - 4;

    return window;
}

void screen_queue_free(struct screen_queue *window)
{
    struct queue_row *current = window->head;
    while (window->head) {
        window->head = current->next;
        queue_row_free(current);
        current = window->head;
    }

    if (window->selected != NULL)
        window->selected = NULL;
    if (window->top_visible != NULL)
        window->top_visible = NULL;

    delwin(window->win);
    free(window);
}

/* Populate the queue window with songs from the current MPD queue */
void screen_queue_populate(struct screen_queue *window)
{
    if (mpd_info->connection == NULL)
        return;

    struct mpd_song *song;
    struct queue_row *row;
    mpd_send_list_queue_meta(mpd_info->connection);

    while ((song = mpd_recv_song(mpd_info->connection)) != NULL) {
        row = queue_row_init(song);
        screen_queue_add_song(window, row->song);
    }

    int i = 1;
    window->bottom_visible = window->head;
    while (window->bottom_visible->next && i < window->max_visible) {
        window->bottom_visible = window->bottom_visible->next;
        ++i;
    }
}

/* Add a song to the current MPD queue */
struct queue_row *screen_queue_add_song(struct screen_queue *window,
                                        struct mpd_song *song)
{
    struct queue_row *row = queue_row_init(song);

    if (!window->head) {
        window->head = row;
        window->selected = window->head;
        window->selected->is_selected = true;
        window->top_visible = row;
    }
    else {
        struct queue_row *current = window->head;
        while (current->next)
            current = current->next;
        current->next = row;
        row->prev = current;
    }

    return row;
}

/* Draw a row on the queue window */
void screen_queue_draw_row(struct screen_queue *window, struct queue_row *row,
                           int begin_y, int begin_x)
{
    if (begin_y >= window->max_visible)
        return;

    int song_label_maxlen = COLS - strlen(row->duration_label) - 1;

    wmove(window->win, begin_y, begin_x);
    wclrtoeol(window->win);

    if (mpd_song_get_id(mpd_info->current_song) == mpd_song_get_id(row->song))
        wattr_on(window->win, A_BOLD, NULL);

    mvwaddnstr(window->win, begin_y, begin_x, row->song_label, song_label_maxlen);
    mvwaddstr(window->win, begin_y, COLS - strlen(row->duration_label), row->duration_label);

    if (row->is_selected)
        mvwchgat(window->win, begin_y, begin_x, -1, A_STANDOUT, 0, NULL);
    wattr_off(window->win, A_BOLD, NULL);
}

/* Draw all nodes in a queue window's list */
void screen_queue_draw_all(struct screen_queue *window)
{
    struct queue_row *current = window->top_visible;

    int i = 0;
    while (current != window->bottom_visible->next) {
        screen_queue_draw_row(window, current, i++, 0);
        current = current->next;
    }
}

void screen_queue_move_cursor(struct screen_queue *window, int direction)
{
    struct queue_row *current = window->selected;

    if (!current && !window->head)
        return;

    /* If nothing is selected but there are list items, select the head */
    if (!current && window->head) {
        window->selected = window->head;
        current = window->selected;
    }

    if (direction == DOWN && current->next) {
        /* If the last line is selected, scroll down */
        if (window->bottom_visible == current) {
            window->top_visible = window->top_visible->next;
            window->bottom_visible = window->bottom_visible->next;
        }
        current->is_selected = false;
        current->next->is_selected = true;
        window->selected = current->next;
    }
    else if (direction == UP && current->prev) {
        /* If the first line is selected, scroll up */
        if (window->top_visible == current) {
            window->top_visible = window->top_visible->prev;
            window->bottom_visible = window->bottom_visible->prev;
        }
        current->is_selected = false;
        current->prev->is_selected = true;
        window->selected = current->prev;
    }

    screen_queue_draw_all(window);
}

/* Scroll the window one page in the specified direction */
void screen_queue_scroll_page(struct screen_queue *window, int direction)
{
    if (window->selected == NULL)
        return;

    for (int i = 0; i < window->max_visible - 1; ++i)
        screen_queue_move_cursor(window, direction);

    screen_queue_draw_all(window);
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