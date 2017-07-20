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
#include "command.h"
#include "status_bar.h"
#include <stdbool.h>
#include <ncurses.h>

struct screen_queue {
    struct list *list;
    unsigned int queue_version;
    int current_song_pos;
    enum mpd_state last_state;
};

struct screen_queue *screen_queue_init();
void screen_queue_free(struct screen_queue *screen_queue);

void screen_queue_populate_list(struct screen_queue *screen_queue);
void screen_queue_update(struct screen_queue *screen_queue);

static inline void screen_queue_draw(struct screen_queue *screen_queue) {
    list_draw(screen_queue->list);
}

static inline void screen_queue_move_cursor(struct screen_queue *screen_queue, enum direction direction) {
    list_move_direction(screen_queue->list, direction);
}

static inline void screen_queue_move_pos(struct screen_queue *screen_queue, enum screen_pos pos) {
    list_move_to_pos(screen_queue->list, pos);
}

static inline void screen_queue_move_screen_pos(struct screen_queue *screen_queue, enum screen_pos pos) {
    list_move_to_screen_pos(screen_queue->list, pos);
}

static inline void screen_queue_scroll_line(struct screen_queue *screen_queue, enum direction direction) {
    list_scroll_line(screen_queue->list, direction);
}

static inline void screen_queue_scroll_page(struct screen_queue *screen_queue, enum direction direction) {
    list_scroll_page(screen_queue->list, direction);
}

static inline void screen_queue_clear(struct screen_queue *screen_queue) {
    list_clear(screen_queue->list);
}

static inline void screen_queue_scroll_half_page(struct screen_queue *screen_queue, enum direction direction) {
    list_scroll_half_page(screen_queue->list, direction);
}

void screen_queue_cmd(command_t cmd, struct screen_queue *screen, struct status_bar *status_bar);


#endif //NCMPCLONE_SCREEN_QUEUE_H
