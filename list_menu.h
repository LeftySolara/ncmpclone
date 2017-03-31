/******************************************************************************
 * list_menu.h : list for holding playlist menu items
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

// TODO: Add colors to menu items

#ifndef NCMPCLONE_LIST_MENU_H
#define NCMPCLONE_LIST_MENU_H

#include <stdbool.h>
#include <ncurses.h>

//struct row {
//    char *text;
//    char *desc;
//    bool selected;
//    struct row *next;
//    struct row *prev;
//};
//
//struct row *row_init(char *text, char *desc);
//void row_free(struct row *row);
//void row_draw(int pos, struct row *row, WINDOW *win);
//
//
//struct list_menu {
//    struct row *head;
//    int length;
//    int selected_index;
//};
//
//struct list_menu *list_menu_init();
//void list_menu_free(struct list_menu *list_menu);
//void list_menu_append(char *text, char *desc, struct list_menu *list_menu);
//void list_menu_erase(int pos, struct list_menu *list_menu);
//void list_menu_clear(struct list_menu *list_menu);
//void list_menu_select_next(struct list_menu *list_menu);
//void list_menu_select_prev(struct list_menu *list_menu);


#endif //NCMPCLONE_LIST_MENU_H
