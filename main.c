/******************************************************************************
 * main.c : application entry point
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
#include "screen_queue.h"

#include <menu.h>
#include <mpd/client.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define MPD_DEFAULT_HOST "localhost"
#define MPD_DEFAULT_PORT 6600
#define MPD_DEFAULT_TIMEOUT 5000

int main() {

    /* Establish a connection with the local MPD server */
    struct mpd_connection *mpd_conn = mpd_connection_new(MPD_DEFAULT_HOST,
                                                         MPD_DEFAULT_PORT,
                                                         MPD_DEFAULT_TIMEOUT);

    if (mpd_connection_get_error(mpd_conn) != MPD_ERROR_SUCCESS) {
        printf("Error connecting to MPD server\n");
        mpd_connection_free(mpd_conn);
        exit(1);
    }

    struct playlist *plist = get_current_playlist(mpd_conn);

    /* Initialize ncurses */
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, 1);
    keypad(stdscr, TRUE);
    refresh();

    struct screen_queue *queue = create_screen_queue(plist);
    nodelay(queue->win, 1);
    keypad(queue->win, TRUE);
    post_menu(queue->menu);
    wrefresh(queue->win);

    int ch;
    while ((ch = wgetch(queue->win)) != KEY_F(1)) {
        switch(ch) {
            case KEY_UP:
                menu_driver(queue->menu, REQ_UP_ITEM);
                break;
            case KEY_DOWN:
                menu_driver(queue->menu, REQ_DOWN_ITEM);
                break;
            case KEY_PPAGE:
                menu_driver(queue->menu, REQ_SCR_UPAGE);
                break;
            case KEY_NPAGE:
                menu_driver(queue->menu, REQ_SCR_DPAGE);
                break;
        }
    }

    free_screen_queue(queue);
    endwin();

    free_playlist(plist);
    mpd_connection_free(mpd_conn);

    return 0;
}
