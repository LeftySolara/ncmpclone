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
#include "queue_window.h"
#include "title_bar.h"

#include <menu.h>
#include <mpd/client.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define MPD_DEFAULT_HOST "localhost"
#define MPD_DEFAULT_PORT 6600
#define MPD_DEFAULT_TIMEOUT 5000
#define KEY_RETURN 10 /* the KEY_ENTER in ncurses doesn't seem to be working */

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
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, 1);
    keypad(stdscr, TRUE);
    refresh();

    struct title_bar *title_bar = title_bar_init("Queue", mpd_conn);
    struct queue_window *queue_window = queue_window_init();

    title_bar_draw(title_bar);

    queue_window_populate(queue_window, mpd_conn);
    queue_window_draw_all(queue_window);

    wrefresh(queue_window->win);
    wrefresh(title_bar->win);

    int ch;
    while ((ch = getch()) != 'q') {
        title_bar_update_volume(title_bar, mpd_conn);
        title_bar_draw(title_bar);
        switch (ch) {
            case KEY_DOWN:
                queue_window_move_cursor(queue_window, CURSOR_MOVE_DOWN);
                wrefresh(queue_window->win);
                break;
            case KEY_UP:
                queue_window_move_cursor(queue_window, CURSOR_MOVE_UP);
                wrefresh(queue_window->win);
                break;
            case KEY_LEFT:
                mpd_run_change_volume(mpd_conn, -1);
                wrefresh(title_bar->win);
                break;
            case KEY_RIGHT:
                mpd_run_change_volume(mpd_conn, 1);
                wrefresh(title_bar->win);
                break;
            case KEY_RETURN:
                mpd_run_play_id(mpd_conn, mpd_song_get_id(queue_window->selected->song));
                break;
            case 'p':
                mpd_run_toggle_pause(mpd_conn);
                break;
            case 's':
                mpd_run_stop(mpd_conn);
                break;
        }
    }

    title_bar_free(title_bar);
    queue_window_free(queue_window);
    endwin();

    mpd_connection_free(mpd_conn);

    return 0;
}
