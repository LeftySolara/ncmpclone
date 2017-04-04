/******************************************************************************
 * queue_list.h : Doubly Linked List for queue menu items
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

#ifndef NCMPCLONE_QUEUE_LIST_H
#define NCMPCLONE_QUEUE_LIST_H

#include <stdlib.h>
#include <stdbool.h>

struct node {
    char *track_label;
    char *duration_label;
    bool is_selected;
    struct node *next;
    struct node *prev;
};

struct node *node_init(char *track_label, char *duration_label);
void node_free(struct node *node);

/*****************************************************************************/

struct list {
    struct node *head;
    size_t length;
};

struct list *list_init();
void list_free(struct list *list);
struct node *list_append(struct list *list, char *track_label, char *duration_label);
void list_erase(struct list *list, size_t pos);
void list_clear(struct list *list);

#endif //NCMPCLONE_QUEUE_LIST_H
