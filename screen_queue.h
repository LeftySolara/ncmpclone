/******************************************************************************
 * screen_queue.h : display current queue to user
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

#include "playlist.h"

#include <menu.h>

struct screen_queue {
    WINDOW *win;
    MENU *menu;
};

struct screen_queue *create_screen_queue(struct playlist *plist);
void free_screen_queue(struct screen_queue *screen);
MENU *create_queue_menu(struct playlist *plist, WINDOW *win);
void free_queue_menu(MENU *queue_menu);
char *get_track_label(struct mpd_song *song);
char *get_track_duration(struct mpd_song *song);

#endif //NCMPCLONE_SCREEN_QUEUE_H
