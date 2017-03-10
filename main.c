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

#include <ncurses.h>
#include <mpd/client.h>
#include <stdio.h>
#include <stdlib.h>

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

    /* Initialize ncurses */
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, 1);
    keypad(stdscr, TRUE);

    int x = getmaxx(stdscr) / 2;
    int y = getmaxy(stdscr) / 2;
    mvprintw(LINES - 1, 0, "Press F1 to exit");
    mvprintw(y, x, "Hello world!");
    refresh();

    int ch;
    while ((ch = getch()) != KEY_F(1)) {
        ;
    }

    endwin();
    mpd_connection_free(mpd_conn);

    return 0;
}
