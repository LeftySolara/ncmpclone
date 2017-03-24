/******************************************************************************
 * list_window.c : window for displaying items in a selectable, menu-like list
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

#include "list_window.h"
#include <stdlib.h>

/* Allocate memory for a list_menu window */
struct list_window *list_window_init()
{
    struct list_window *list_window = malloc(sizeof(struct list_window *));
    int width = getmaxx(stdscr);

    list_window->win = newwin(LINES - 2, width, 2, 0);
    list_window->queue_menu = list_menu_init();
}

/* Free memory used by a list_menu window */
void list_window_free(struct list_window *list_window)
{
    delwin(list_window->win);
    list_menu_free(list_window->queue_menu);
    free(list_window);
}

/* Draw all rows to the screen */
void list_window_draw(struct list_window *list_window)
{

}
