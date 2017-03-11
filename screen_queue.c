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
#include <stdlib.h>
#include <mpd/client.h>

/* Create an ncurses menu containing the current queue. If NULL is passed for
 * win, use stdscr */
MENU *create_queue_menu(struct playlist *plist, WINDOW *win)
{
    if (plist == NULL)
        return NULL;
    if (win == NULL)
        win = stdscr;

    const int num_songs = plist->song_count;
    char *artist;
    char *title;
    MENU *menu;
    ITEM **queue_menu_items;

    queue_menu_items = (ITEM **)calloc(num_songs + 1, sizeof(plist->songs[0]));
    queue_menu_items[num_songs] = (ITEM *)NULL;
    for (int i = 0; i < num_songs; ++i) {
        artist = mpd_song_get_tag(plist->songs[i], MPD_TAG_ARTIST, 0);
        title = mpd_song_get_tag(plist->songs[i], MPD_TAG_TITLE, 0);
        queue_menu_items[i] = new_item(artist, title);
    }

    menu = new_menu(queue_menu_items);
    set_menu_mark(menu, "");
    set_menu_sub(menu, win);

    return menu;
}
