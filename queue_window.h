/******************************************************************************
 * window_queue.h : main window for the queue screen
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

struct row {
    char *song_label;
    char *duration_label;
    bool selected;
    struct row *prev;
    struct row *next;
};

struct row *row_init(char *song, char *duration);
void row_free(struct row *row);
void row_draw(struct row *row, WINDOW *win, int begin_y, int begin_x);

struct queue_window {
    WINDOW *win;
    struct row *head;
    struct row *selected;
};

struct queue_window *queue_window_init();
void queue_window_free(struct queue_window *window);
void queue_window_add_row(struct queue_window *window, char *song, char *duration);



#endif //NCMPCLONE_WINDOW_QUEUE_H
