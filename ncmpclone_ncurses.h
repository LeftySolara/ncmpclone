/******************************************************************************
 * ncmpclone_ncurses.h : basic functions for interacting with ncurses
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

#ifndef NCMPCLONE_NCMPCLONE_NCURSES_H
#define NCMPCLONE_NCMPCLONE_NCURSES_H

#include "mpd_info.h"

char *create_track_label(struct mpd_song *song);
char *create_duration_label(struct mpd_song *song);
char *key_to_str(int key);

#endif //NCMPCLONE_NCMPCLONE_NCURSES_H
