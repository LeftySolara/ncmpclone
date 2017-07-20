/******************************************************************************
 * screen_browse.h : screen for browsing the music library
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

#ifndef NCMPCLONE_SCREEN_BROWSE_H
#define NCMPCLONE_SCREEN_BROWSE_H

#include "list.h"
#include "command.h"
#include "status_bar.h"

struct screen_browse {
    struct list *list;
};

struct screen_browse *screen_browse_init();
void screen_browse_free(struct screen_browse *screen);

void screen_browse_populate(struct screen_browse *screen);
void screen_browse_cmd(command_t cmd, struct screen_browse *screen,
                       struct status_bar *status_bar);


static inline void screen_browse_draw(struct screen_browse *screen) {
    list_draw(screen->list);
}

static inline void screen_browse_move_cursor(struct screen_browse *screen, enum direction direction) {
    list_move_direction(screen->list, direction);
}

static inline void screen_browse_move_pos(struct screen_browse *screen, enum screen_pos pos) {
    list_move_to_pos(screen->list, pos);
}

static inline void screen_browse_move_screen_pos(struct screen_browse *screen, enum screen_pos pos) {
    list_move_to_screen_pos(screen->list, pos);
}

static inline void screen_browse_scroll_line(struct screen_browse *screen, enum direction direction) {
    list_scroll_line(screen->list, direction);
}

static inline void screen_browse_scroll_page(struct screen_browse *screen, enum direction direction) {
    list_scroll_page(screen->list, direction);
}

static inline void screen_browse_clear(struct screen_browse *screen) {
    list_clear(screen->list);
}

static inline void screen_browse_scroll_half_page(struct screen_browse *screen, enum direction direction) {
    list_scroll_half_page(screen->list, direction);
}

#endif //NCMPCLONE_SCREEN_BROWSE_H
