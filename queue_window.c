/******************************************************************************
 * queue_window.c : main window for the queue screen
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

struct queue_window *queue_window_init()
{
    struct queue_window *window = malloc(sizeof(*window));

    /* The window starts just under the title bar and uses the full width
     * of the screen */
    window->win = newwin(LINES - 2, COLS, 2, 0);
    window->track_list = list_init();

    return window;
}

void queue_window_free(struct queue_window *window)
{
    delwin(window->win);
    list_free(window->track_list);
    free(window);
}

void queue_window_add_row(struct queue_window *window, char *track_label,
                          char *duration_label)
{
    struct node *node = list_append(window->track_list, track_label, duration_label);
    if (window->track_list->length == 1)
        window->selected = window->track_list->head;
}

/* Draw a row on the window using data from a list node */
void queue_window_draw_row(struct node *node, WINDOW *win, int begin_y, int begin_x)
{
    int song_label_maxlen = COLS - strlen(node->duration_label) - 1;

    wmove(win, begin_y, begin_x);
    wclrtoeol(win);

    mvwaddnstr(win, begin_y, begin_x, node->track_label, song_label_maxlen);
    mvwaddstr(win, begin_y, COLS - strlen(node->duration_label), node->duration_label);

    if (node->is_selected) {
        mvwchgat(win, begin_y, begin_x, -1, A_STANDOUT, 0, NULL);
    }
}

/* Draw all nodes in a queue window's list */
void queue_window_draw_all(struct queue_window *window)
{
    struct node *current = window->track_list->head;

    int i = 0;
    while (current) {
        queue_window_draw_row(current, window->win, i++, 0);
        current = current->next;
    }
}

void queue_window_move_curs(struct queue_window *window, int direction)
{
    struct node *current = window->selected;

    if (!current && window->track_list->length == 0)
        return;

    /* If nothing is selected but there are list items, select the head */
    if (!current && window->track_list->length > 0) {
        window->selected = window->track_list->head;
        current = window->selected;
    }

    if (direction == CURS_MOVE_DOWN && current->next) {
        current->is_selected = false;
        current->next->is_selected = true;
        window->selected = current->next;
    }
    else if (direction == CURS_MOVE_UP && current->prev) {
        current->is_selected = false;
        current->prev->is_selected = true;
        window->selected = current->prev;
    }

    queue_window_draw_all(window);
    wrefresh(window->win);
}
