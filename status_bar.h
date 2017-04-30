/******************************************************************************
 * status_bar.h : display label and progress for currently playing track
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

#ifndef NCMPCLONE_STATUS_BAR_H
#define NCMPCLONE_STATUS_BAR_H

#include <ncurses.h>
#include <mpd/client.h>

struct status_bar {
    WINDOW *win;
    struct mpd_connection *mpd_conn;
    struct mpd_status *mpd_status;
};

struct status_bar *status_bar_init(struct mpd_connection *mpd_conn);
void status_bar_free(struct status_bar *bar);
void status_bar_draw(struct status_bar *bar);
void status_bar_update(struct status_bar *bar);
char *create_song_label(struct mpd_song *song);

#endif //NCMPCLONE_STATUS_BAR_H
