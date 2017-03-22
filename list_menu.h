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

struct row {
    char *text;
    char *desc;
    bool selected;
    struct row *next;
    struct row *prev;
};

struct row *row_init(char *text, char *desc);
void row_free(struct row *row);


struct list {
    struct row *head;
    int length;
};

struct list *list_init();
void list_free(struct list *list);
void list_append(char *text, char *desc, struct list *list);
void list_erase(int pos, struct list *list);
void list_clear(struct list *list);


#endif //NCMPCLONE_LIST_MENU_H
