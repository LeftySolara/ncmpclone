/******************************************************************************
 * screen_queue.c : display current queue to user
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

#include "screen_queue.h"

#include <mpd/client.h>
#include <stdlib.h>
#include <string.h>

/* Create an ncurses menu containing the current queue. If NULL is passed for
 * win, use stdscr */
MENU *create_queue_menu(struct playlist *plist, WINDOW *win)
{
    if (plist == NULL)
        return NULL;
    if (win == NULL)
        win = stdscr;

    const int num_songs = plist->song_count;
    const char *label;
    const char *duration;
    MENU *menu;
    ITEM **queue_menu_items;

    queue_menu_items = calloc(num_songs, sizeof(ITEM *));
    for (int i = 0; i < num_songs; ++i) {
        label = get_track_label(plist->songs[i]);
        duration = get_track_duration(plist->songs[i]);
        queue_menu_items[i] = new_item(label, duration);
    }

    menu = new_menu(queue_menu_items);
    set_menu_mark(menu, "");
    set_menu_sub(menu, win);

    return menu;
}

/* Create a song label of the format "artist - title" */
char *get_track_label(struct mpd_song *song)
{
    const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
    const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
    char *buffer = malloc(strlen(artist) + strlen(title) + 4);

    if (buffer == NULL) {
        printf("Cannot create track label. No memory available.");
    }
    else {
        strcpy(buffer, artist);
        strcat(buffer, " - ");
        strcat(buffer, title);
    }

    return buffer;
}

/* Create a human-readable string representing a song's duration */
char *get_track_duration(struct mpd_song *song)
{
    const int buf_size = 7;
    char *buffer = calloc(buf_size, sizeof(char));

    if (buffer == NULL)
        printf("Cannot create duration string. Not enough memory available.");
    else {
        int minutes = mpd_song_get_duration(song) / 60;
        int seconds = mpd_song_get_duration(song) % 60;
        snprintf(buffer, buf_size, "%d:%02d", minutes, seconds);
    }

    return buffer;
}
