/******************************************************************************
 * list.c : functionality for displaying lists on the screen
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

#include "list.h"
#include <stdlib.h>
#include <string.h>

struct list_item *list_item_init(char *left, char *right, bool bold, bool highlight)
{
    struct list_item *item = malloc(sizeof(*item));
    item->left_str = left;
    item->right_str = right;
    item->bold = bold;
    item->highlight = highlight;
    item->prev = NULL;
    item->next = NULL;
}

void list_item_free(struct list_item *item)
{
    item->prev = NULL;
    item->next = NULL;

    if (item->left_str)
        free(item->left_str);
    if (item->right_str)
        free(item->right_str);

    free(item);
}

/*****************************************************************************/

struct list *list_init()
{
    struct list *list = malloc(sizeof(*list));
    list->win = newwin(LINES - 4, COLS, 2, 0);
    list->head = NULL;
    list->selected = NULL;
    list->top_visible = list->head;
    list->bottom_visible = list->head;
    list->max_visible = LINES - 4;
    list->selected_index = 0;

    return list;
}

void list_free(struct list *list)
{
    list_clear(list);
    delwin(list->win);
    free(list);
}

/* Add an item to the end of the list */
void list_append_item(struct list *list, char *left, char *right, bool bold)
{
    struct list_item *item = list_item_init(left, right, bold, false);

    if (!list->head) {
        list->head = item;
        list->selected = list->head;
        list->selected->highlight = true;
        list->top_visible = item;
    }
    else {
        struct list_item *current = list->head;
        while (current->next)
            current = current->next;
        current->next = item;
        item->prev = current;
    }
}

/* Remove the item currently selected by the user */
void list_remove_selected(struct list *list)
{
    if (!list->head)  // List is empty
        return;

    if (list->selected == list->head) {
        // Only one item in the list
        if (!list->head->next) {
            list_item_free(list->head);
            list->head = NULL;
            list->selected = NULL;
            list->top_visible = NULL;
            list->selected_index = 0;
        }
        else {
            struct list_item *current = list->head;
            list->head = list->head->next;
            list->selected = list->head;
            list->top_visible = list->head;
            list->selected_index = 0;
            list_item_free(current);
        }
    }
    else {
        struct list_item *current = list->selected;
        current->prev->next = current->next;
        current->next->prev = current->prev;

        if (current->next)
            list->selected = current->next;
        else
            list->selected = current->prev;

        list_item_free(current);
    }
}

void list_clear(struct list *list)
{
    struct list_item *current = list->head;
    while (list->head) {
        list->head = current->next;
        list_item_free(current);
        current = list->head;
    }

    if (list->selected)
        list->selected = NULL;
    if (list->top_visible)
        list->top_visible = NULL;
    if (list->bottom_visible)
        list->bottom_visible = NULL;

    list_draw(list);
}

void list_draw_item(struct list *list, struct list_item *item, int begin_y)
{
    // Don't draw past the end of the window
    if (begin_y >= list->max_visible)
        return;

    // We want to truncate the left string to leave room for
    // the right-justified string
    int left_item_maxlen = COLS - strlen(item->right_str) - 1;
    wmove(list->win, begin_y, 0);
    wclrtoeol(list->win);

    if (item->bold)
        wattr_on(list->win, A_BOLD, NULL);
    if (item->highlight)
        wattr_on(list->win, A_STANDOUT, NULL);

    mvwaddnstr(list->win, begin_y, 0, item->left_str, left_item_maxlen);
    mvwaddstr(list->win, begin_y, COLS - strlen(item->right_str), item->right_str);

    int gap = COLS - strlen(item->left_str) - strlen(item->right_str);
    if (item->highlight)
        mvwchgat(list->win, begin_y, strlen(item->left_str), gap, A_STANDOUT, 0, NULL);

    wattr_off(list->win, A_BOLD, NULL);
    wattr_off(list->win, A_STANDOUT, NULL);
}

void list_draw(struct list *list)
{
    wclear(list->win);
    struct list_item *current = list->top_visible;
    list_find_bottom_visible(list);

    int i = 0;
    while (current) {
        list_draw_item(list, current, i++);
        current = current->next;
    }
}

void list_move_direction(struct list *list, enum direction direction)
{
    struct list_item *current = list->selected;
    list_find_bottom_visible(list);

    if (!current && !list->head)
        return;

    /* If nothing is selected but list items exist, select the head */
    if (!current && list->head) {
        list->selected = list->head;
        list->selected_index = 0;
        current = list->selected;
    }

    if (direction == DOWN && current->next) {
        /* If the last visible line is selected, scroll down */
        if (list->bottom_visible == current) {
            list->top_visible = list->top_visible->next;
            list->bottom_visible = list->bottom_visible->next;
        }
        current->highlight = false;
        current->next->highlight = true;
        list->selected = current->next;
        ++list->selected_index;
    }
    else if (direction == UP && current->prev) {
        /* If the first line is selected, scroll up */
        if (list->top_visible == current) {
            list->top_visible = list->top_visible->prev;
            list->bottom_visible = list->bottom_visible->prev;
        }
        current->highlight = false;
        current->prev->highlight = true;
        list->selected = current->prev;
        --list->selected_index;
    }

     list_draw(list);
}

/* Move the cursor to the top or bottom of the list */
void list_move_to_pos(struct list *list, enum screen_pos pos)
{
    if (!list->head || pos == MID)
        return;

    if (pos == TOP) {
        while (list->selected != list->head)
            list_move_direction(list, UP);
    }
    else if (pos == BOT) {
        while (list->selected->next)
            list_move_direction(list, DOWN);
    }
}

/* Move the cursor to the top, middle or bottom of the currently visible items */
void list_move_to_screen_pos(struct list *list, enum screen_pos pos)
{
    if (!list->head)
        return;

    if (pos == TOP) {
        while (list->selected != list->top_visible)
            list_move_direction(list, UP);
    }
    else if (pos == BOT) {
        while (list->selected != list->bottom_visible)
            list_move_direction(list, DOWN);
    }
    else if (pos == MID) {
        int midpoint = getmaxy(list->win) / 2;
        list_move_to_screen_pos(list, TOP);

        for (int i = 0; i < midpoint; ++i)
            list_move_direction(list, DOWN);
    }
}

void list_scroll_page(struct list *list, enum direction direction)
{
    if (list->selected == NULL)
       return;

    for (int i = 0; i < list->max_visible - 1; ++i)
        list_move_direction(list, direction);

    list_draw(list);
}

void list_find_bottom_visible(struct list *list)
{
    if (!list->head) {
        list->bottom_visible = NULL;
        return;
    }

    int i = 1;
    list->bottom_visible = list->top_visible;
    while (list->bottom_visible->next && i < list->max_visible) {
        list->bottom_visible = list->bottom_visible->next;
        ++i;
    }
}
