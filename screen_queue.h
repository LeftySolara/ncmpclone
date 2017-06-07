/******************************************************************************
 * queue_window.h : main window for the queue screen
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

#ifndef NCMPCLONE_SCREEN_QUEUE_H
#define NCMPCLONE_SCREEN_QUEUE_H

#include "mpd_info.h"
#include "list.h"
#include <stdbool.h>
#include <ncurses.h>

struct screen_queue {
    struct list *list;
};

struct screen_queue *screen_queue_init();
void screen_queue_free(struct screen_queue *screen_queue);

void screen_queue_populate_list(struct screen_queue *screen_queue);
char *create_track_label(struct mpd_song *song);
char *create_duration_label(struct mpd_song *song);

static inline void screen_queue_draw(struct screen_queue *screen_queue) {
    list_draw(screen_queue->list);
}

static inline void screen_queue_move_cursor(struct screen_queue *screen_queue, enum direction direction) {
        list_move_cursor(screen_queue->list, direction);
}

static inline void screen_queue_scroll_page(struct screen_queue *screen_queue, enum direction direction) {
    list_scroll_page(screen_queue->list, direction);
}


#endif //NCMPCLONE_SCREEN_QUEUE_H
