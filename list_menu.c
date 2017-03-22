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

struct row *row_init(char *text, char *desc)
{
    struct row *row = malloc(sizeof(struct row *));

    row->text = text;
    row->desc = desc;
    row->next = NULL;
    row->prev = NULL;
    row->selected = false;

    return row;
}

void row_free(struct row *row)
{
    row->prev = NULL;
    row->next = NULL;
    free(row);
}

struct list *list_init()
{
    struct list *list = malloc(sizeof(struct list *));
    list->head = NULL;
    list->length = 0;

    return list;
}

void list_free(struct list *list)
{
    list_clear(list);
    free(list);
}

/* Add a row to the end of the list */
void list_append(char *text, char *desc, struct list *list)
{
    struct row *row = row_init(text, desc);

    if (list->head == NULL)
        list->head = row;
    else {
        struct row *current = list->head;
        while (current->next)
            current = current->next;
        current->next = row;
        row->prev = current;
    }
    ++list->length;
}

/* Remove the item at position pos from the list */
void list_erase(int pos, struct list *list)
{
    if (pos > list->length)
        return;

    struct row *current = list->head;
    if (pos == 0) {  /* Item is the head */
        list->head = current->next;
        list->head->prev = NULL;
    }
    else {
        for (int i = 0; i < pos; ++i)
            current = current->next;

        current->prev->next = current->next;
        if (current->next)  /* Not at the tail */
            current->next->prev = current->prev;
    }
    row_free(current);
    --list->length;
}

/* Remove all items from list. This does not free the memory used by the list */
void list_clear(struct list *list)
{
    struct row *current = list->head;
    while (list->head) {
        list->head = current->next;
        row_free(current);
        current = list->head;
        --list->length;
    }
}
