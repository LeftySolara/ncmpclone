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

// TODO: Write function for scaling screen on window resize

#include "screen_queue.h"

#include <mpd/client.h>
#include <stdlib.h>
#include <string.h>

///* Create a screen to display the current queue. */
//struct screen_queue *create_screen_queue(struct playlist *plist)
//{
//    if (plist == NULL)
//        return NULL;
//
//    struct screen_queue *screen;
//
//    screen = malloc(sizeof(*screen));
//    WINDOW *win = newwin(LINES - 5, 0, 2, 0);
//    MENU *menu = create_queue_menu(plist, win);
//    set_menu_format(menu, LINES - 5, 0);
//
//    screen->win = win;
//    screen->menu = menu;
//
//    return screen;
//}
//
///* Free memory used by the queue screen */
//void free_screen_queue(struct screen_queue *screen)
//{
//    unpost_menu(screen->menu);
//    free_queue_menu(screen->menu);
//    delwin(screen->win);
//    free(screen);
//}
//
///* Create an ncurses menu containing the current queue. If NULL is passed for
// * win, use stdscr */
//MENU *create_queue_menu(struct playlist *plist, WINDOW *win)
//{
//    if (plist == NULL)
//        return NULL;
//
//    const int num_songs = plist->song_count;
//    const char *label;
//    const char *duration;
//    MENU *menu;
//    ITEM **queue_menu_items;
//
//    queue_menu_items = calloc(num_songs, sizeof(ITEM *));
//    for (int i = 0; i < num_songs; ++i) {
//        label = get_track_label(plist->songs[i]);
//        duration = get_track_duration(plist->songs[i]);
//        queue_menu_items[i] = new_item(label, duration);
//    }
//
//    menu = new_menu(queue_menu_items);
//    set_menu_mark(menu, "");
//    set_menu_win(menu, win);
//    set_menu_sub(menu, win);
//
//    return menu;
//}
//
///* Free memory used by a queue menu */
//void free_queue_menu(MENU *queue_menu)
//{
//    ITEM **queue_menu_items = menu_items(queue_menu);
//    const int item_count = queue_menu->nitems;
//
//    for (int i = 0; i < item_count; ++i)
//        free_item(queue_menu_items[i]);
//    free_menu(queue_menu);
//}
//
///* Create a song label of the format "artist - title" */
//char *get_track_label(struct mpd_song *song)
//{
//    const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
//    const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
//    char *buffer = malloc(strlen(artist) + strlen(title) + 4);
//
//    if (buffer == NULL) {
//        printf("Cannot create track label. No memory available.");
//    }
//    else {
//        strcpy(buffer, artist);
//        strcat(buffer, " - ");
//        strcat(buffer, title);
//    }
//
//    return buffer;
//}
//
///* Create a human-readable string representing a song's duration */
//char *get_track_duration(struct mpd_song *song)
//{
//    const int buf_size = 7;
//    char *buffer = calloc(buf_size, sizeof(char));
//
//    if (buffer == NULL)
//        printf("Cannot create duration string. Not enough memory available.");
//    else {
//        int minutes = mpd_song_get_duration(song) / 60;
//        int seconds = mpd_song_get_duration(song) % 60;
//        snprintf(buffer, buf_size, "%3d:%02d", minutes, seconds);
//    }
//
//    return buffer;
//}
