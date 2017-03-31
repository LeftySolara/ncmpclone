/******************************************************************************
 * window_queue.c : main window for the queue screen
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

#include "queue_window.h"
#include <stdlib.h>
#include <string.h>

/* Allocate and initialize memory for a row */
struct row *row_init(char *song, char *duration)
{
    struct row *new_row = malloc(sizeof(struct row *));
    new_row->song_label = strdup(song);
    new_row->duration_label = strdup(duration);
    new_row->selected = false;

    return new_row;
}

/* Free memory used by a row */
void row_free(struct row *row)
{
    free(row->song_label);
    free(row->duration_label);
    free(row);
}

/* Draw a row to the given window */
void row_draw(struct row *row, WINDOW *win, int begin_y, int begin_x)
{
    int song_label_maxlen = COLS - strlen(row->duration_label) - 1;
    mvwaddnstr(win, begin_y, begin_x, row->song_label, song_label_maxlen);
    mvwaddstr(win, begin_y, COLS - strlen(row->duration_label), row->duration_label);
    wrefresh(win);
}

/*****************************************************************************/

struct queue_window *queue_window_init()
{
    struct queue_window *window = malloc(sizeof(struct queue_window *));
    window->head = NULL;
    window->selected = NULL;

    /* The window starts just under the title bar and uses the full width
     * of the screen */
    window->win = newwin(LINES - 2, COLS, 2, 0);
}

void queue_window_free(struct queue_window *window)
{
    delwin(window->win);
    window->selected = NULL;

    /* Free memory from all the rows */
    struct row *temp = window->head;
    while (window->head) {
        window->head = window->head->next;
        row_free(temp);
        temp = window->head;
    }

    free(window);
}

void queue_window_add_row(struct queue_window *window, char *song, char *duration)
{
    struct row *row = row_init(song, duration);

    if (!window->head)
        window->head = row;
    else {
        struct row *temp = window->head;
        while (temp->next)
            temp = temp->next;
        temp->next = row;
        row->prev = temp;
    }
}
