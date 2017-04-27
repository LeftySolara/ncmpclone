/******************************************************************************
 * queue_window.h : main window for the queue screen
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

#ifndef NCMPCLONE_WINDOW_QUEUE_H
#define NCMPCLONE_WINDOW_QUEUE_H

#include <stdbool.h>
#include <ncurses.h>
#include <mpd/client.h>

#define DOWN 1
#define UP 2

struct queue_row {
    struct mpd_song *song;
    char *song_label;
    char *duration_label;
    bool is_selected;

    struct queue_row *next;
    struct queue_row *prev;
};

struct queue_window {
    WINDOW *win;
    struct queue_row *head;
    struct queue_row *selected;
    struct queue_row *top_visible;
    struct queue_row *bottom_visible;
    int max_visible;
};

struct queue_row *queue_row_init(struct mpd_song *song);
void queue_row_free(struct queue_row *row);

struct queue_window *queue_window_init();
void queue_window_free(struct queue_window *window);

void queue_window_populate(struct queue_window *window, struct mpd_connection *conn);
struct queue_row *queue_window_add_song(struct queue_window *window, struct mpd_song *song);
void queue_window_draw_row(struct queue_window *window, struct queue_row *row, int begin_y, int begin_x);
void queue_window_draw_all(struct queue_window *window);
void queue_window_move_cursor(struct queue_window *window, int direction);
void queue_window_scroll_page(struct queue_window *window, int direction);

char *create_track_label(struct mpd_song *song);
char *create_duration_label(struct mpd_song *song);


#endif //NCMPCLONE_WINDOW_QUEUE_H
