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

#include "queue_list.h"

#include <stdbool.h>
#include <ncurses.h>

struct queue_window {
    WINDOW *win;
    struct node *selected;
    struct list *track_list;
};

struct queue_window *queue_window_init();
void queue_window_free(struct queue_window *window);
void queue_window_add_row(struct queue_window *window, char *track_label, char *duration_label);
void queue_window_draw_row(struct node *node, WINDOW *win, int begin_y, int begin_x);
void queue_window_draw_all(struct queue_window *window);


#endif //NCMPCLONE_WINDOW_QUEUE_H
