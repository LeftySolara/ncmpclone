/******************************************************************************
 * playlist.c : store and manipulate playlist information
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

#include "playlist.h"

#include <stdlib.h>

/* Get a list of songs from the current queue */
struct playlist *get_current_playlist(struct mpd_connection *conn)
{
    struct playlist *plist;
    struct mpd_song *song;

    plist = malloc(sizeof(*plist));
    plist->name = "queue";
    plist->song_count = 0;
    plist->songs = NULL;

    mpd_send_list_queue_meta(conn);
    while ((song = mpd_recv_song(conn)) != NULL) {
        ++plist->song_count;
        plist->songs = realloc(plist->songs, plist->song_count * sizeof(plist->songs));
        plist->songs[plist->song_count - 1] = song;
    }

    return plist;
}

/* Free memory used by a playlist */
void free_playlist(struct playlist *plist)
{
    // TODO: Write function for removing individual songs from a playlist
    int song_count = plist->song_count;
    for (int i = 0; i < song_count; ++i)
        mpd_song_free(plist->songs[i]);
    free(plist->songs);
}
