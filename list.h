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

#ifndef NCMPCLONE_LIST_H
#define NCMPCLONE_LIST_H

#include <ncurses.h>

enum direction {DOWN, UP};
enum screen_pos {TOP, MID, BOT};

struct list_item {
    char *left_str;
    char *right_str;
    bool bold;          // For the currently playing item
    bool highlight;     // For the currently selected item
    struct list_item *prev;
    struct list_item *next;
};

struct list {
    WINDOW *win;

    int max_visible;
    unsigned int selected_index;
    struct list_item *head;
    struct list_item *selected;
    struct list_item *top_visible;
    struct list_item *bottom_visible;

    bool range_select;
    int range_head_index;
    struct list_item *range_head;
};

struct list_item *list_item_init(char *left, char *right, bool bold, bool highlight);
void list_item_free(struct list_item *item);

struct list *list_init();
void list_free(struct list *list);

void list_append_item(struct list *list, char *left, char *right, bool bold);
void list_remove_selected(struct list *list);
void list_clear(struct list *list);

void list_draw_item(struct list *list, struct list_item *item, int begin_y);
void list_draw(struct list *list);

void list_move_direction(struct list *list, enum direction direction);
void list_move_to_pos(struct list *list, enum screen_pos pos);
void list_move_to_screen_pos(struct list *list, enum screen_pos pos);
void list_scroll_line(struct list *list, enum direction direction);
void list_scroll_page(struct list *list, enum direction direction);

void list_find_bottom_visible(struct list *list);
void list_toggle_range(struct list *list);

#endif //NCMPCLONE_LIST_H
