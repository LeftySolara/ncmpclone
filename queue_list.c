/******************************************************************************
 * queue_list.c : Doubly Linked List for queue menu items
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

#include "queue_list.h"
#include <string.h>

/* Create a node and initialize its data */
struct node *node_init(char *track_label, char *duration_label)
{
    struct node *node = malloc(sizeof(*node));
    node->track_label = strdup(track_label);
    node->duration_label = strdup(duration_label);
    node->is_selected = false;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

/* Free all memory used by a node */
void node_free(struct node *node)
{
    node->next = NULL;
    node->prev = NULL;
    free(node->track_label);
    free(node->duration_label);
    free(node);
}

/* Create an empty list */
struct list *list_init()
{
    struct list *list = malloc(sizeof(struct list *));
    list->head = NULL;
    list->length = 0;

    return list;
}

/* Delete all nodes from list and free all used memory */
void list_free(struct list *list)
{
    list_clear(list);
    free(list);
}

/* Append a node with data to the end of the list */
struct node *list_append(struct list *list, char *track_label, char *duration_label)
{
    struct node *node = node_init(track_label, duration_label);

    if (!list->head) {
        node->is_selected = true;
        list->head = node;
    }
    else {
        struct node *current = list->head;
        while (current->next)
            current = current->next;
        current->next = node;
        node->prev = current;
    }
    ++list->length;

    return node;
}

/* Remove a node from position pos */
void list_erase(struct list *list, size_t pos)
{
    if (pos > list->length)
        return;

    struct node *current = list->head;
    if (pos == 0) {
        list->head = current->next;
        list->head->prev = NULL;
        list->head->is_selected = true;
        node_free(current);
    }
    else {
        for (int i = 0; i < pos; ++i)
            current = current->next;

        current->prev->next = current->next;
        if (current->next)
            current->next->prev = current->prev;

        /* If there's another node after the one being deleted, select it */
        if (current->is_selected && current->next)
            current->next->is_selected = true;
        else if (current->is_selected && current->prev) /* node is last in list */
            current->prev->is_selected = true;

        node_free(current);
    }
    --list->length;
}

/* Remove all nodes from a list. This does not free the list's memory */
void list_clear(struct list *list)
{
    struct node *current = list->head;
    while (list->head) {
        list->head = current->next;
        node_free(current);
        current = list->head;
        --list->length;
    }
}
