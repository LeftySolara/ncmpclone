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
#include <time.h>

struct status_bar {
    WINDOW *win;
    char *progress_label;
    char *notification;
    time_t notify_end;
};

struct status_bar *status_bar_init();
void status_bar_free(struct status_bar *bar);
void status_bar_draw_progress(struct status_bar *bar);
void status_bar_draw(struct status_bar *bar);
char *create_song_label(struct mpd_song *song);
char *create_progress_label(char *buffer);

#endif //NCMPCLONE_STATUS_BAR_H
