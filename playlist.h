/******************************************************************************
 * playlist.h : store and manipulate playlist information
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

#ifndef NCMPCLONE_PLAYLIST_H
#define NCMPCLONE_PLAYLIST_H

#include <mpd/client.h>

struct playlist {
    char *name;
    unsigned int song_count;
    struct mpd_song **songs;
};

struct playlist *get_current_playlist(struct mpd_connection *conn);
void free_playlist(struct playlist *plist);

#endif //NCMPCLONE_PLAYLIST_H
