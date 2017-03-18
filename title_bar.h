/******************************************************************************
 * title_bar.h : display screen title and current volume to the user
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

#ifndef NCMPCLONE_TITLE_BAR_H
#define NCMPCLONE_TITLE_BAR_H

#include <ncurses.h>
#include <mpd/client.h>

struct title_bar {
    WINDOW *win;
    struct mpd_status *status;
    char *title;
    int width;
    char volume_label[12];
};

struct title_bar *title_bar_init(char *title, struct mpd_connection *conn);
void title_bar_free(struct title_bar *title_bar);
void title_bar_update_volume(struct title_bar *title_bar, struct mpd_connection *conn);
void title_bar_draw(struct title_bar *title_bar);

#endif //NCMPCLONE_TITLE_BAR_H
