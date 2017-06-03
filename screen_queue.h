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

#ifndef NCMPCLONE_SCREEN_QUEUE_H
#define NCMPCLONE_SCREEN_QUEUE_H

#include "mpd_info.h"
#include <stdbool.h>
#include <ncurses.h>

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

struct screen_queue {
    WINDOW *win;
    struct queue_row *head;
    struct queue_row *selected;
    struct queue_row *top_visible;
    struct queue_row *bottom_visible;
    int max_visible;
};

struct queue_row *queue_row_init(struct mpd_song *song);
void queue_row_free(struct queue_row *row);

struct screen_queue *screen_queue_init();
void screen_queue_free(struct screen_queue *window);

void screen_queue_populate(struct screen_queue *window);
struct queue_row *screen_queue_add_song(struct screen_queue *window,
                                        struct mpd_song *song);
void screen_queue_draw_row(struct screen_queue *window, struct queue_row *row,
                           int begin_y, int begin_x);
void screen_queue_draw_all(struct screen_queue *window);
void screen_queue_move_cursor(struct screen_queue *window, int direction);
void screen_queue_scroll_page(struct screen_queue *window, int direction);

char *create_track_label(struct mpd_song *song);
char *create_duration_label(struct mpd_song *song);


#endif //NCMPCLONE_SCREEN_QUEUE_H
