/******************************************************************************
 * list_menu.c : list for holding playlist menu items
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

#include "list_menu.h"
#include <stdlib.h>

//struct row *row_init(char *text, char *desc)
//{
//    struct row *row = malloc(sizeof(struct row *));
//
//    row->text = text;
//    row->desc = desc;
//    row->next = NULL;
//    row->prev = NULL;
//    row->selected = false;
//
//    return row;
//}
//
//void row_free(struct row *row)
//{
//    row->prev = NULL;
//    row->next = NULL;
//    free(row);
//}
//
///* Draw a row on the given window at row number pos */
//void row_draw(int pos, struct row *row, WINDOW *win)
//{
//    if (row->selected)
//        ; /* TODO: set colors here */
//    mvwaddstr(win, pos, 0, row->text);
//}
//
//struct list_menu *list_menu_init()
//{
//    struct list_menu *list_menu = malloc(sizeof(struct list_menu *));
//    list_menu->head = NULL;
//    list_menu->length = 0;
//
//    return list_menu;
//}
//
//void list_menu_free(struct list_menu *list_menu)
//{
//    list_menu_clear(list_menu);
//    free(list_menu);
//}
//
///* Add a row to the end of the list_menu */
//void list_menu_append(char *text, char *desc, struct list_menu *list_menu)
//{
//    struct row *row = row_init(text, desc);
//
//    if (list_menu->head == NULL)
//        list_menu->head = row;
//    else {
//        struct row *current = list_menu->head;
//        while (current->next)
//            current = current->next;
//        current->next = row;
//        row->prev = current;
//    }
//    ++list_menu->length;
//}
//
///* Remove the item at position pos from the list_menu */
//void list_menu_erase(int pos, struct list_menu *list_menu)
//{
//    if (pos > list_menu->length)
//        return;
//
//    struct row *current = list_menu->head;
//    if (pos == 0) {  /* Item is the head */
//        list_menu->head = current->next;
//        list_menu->head->prev = NULL;
//    }
//    else {
//        for (int i = 0; i < pos; ++i)
//            current = current->next;
//
//        current->prev->next = current->next;
//        if (current->next)  /* Not at the tail */
//            current->next->prev = current->prev;
//    }
//    row_free(current);
//    --list_menu->length;
//}
//
///* Remove all items from list_menu. This does not free the memory used by the list_menu */
//void list_menu_clear(struct list_menu *list_menu)
//{
//    struct row *current = list_menu->head;
//    while (list_menu->head) {
//        list_menu->head = current->next;
//        row_free(current);
//        current = list_menu->head;
//        --list_menu->length;
//    }
//}
