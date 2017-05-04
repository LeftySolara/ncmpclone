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
#include "status_bar.h"

#include <menu.h>
#include <mpd/client.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define MPD_DEFAULT_HOST "localhost"
#define MPD_DEFAULT_PORT 6600
#define MPD_DEFAULT_TIMEOUT 5000
#define KEY_RETURN 10 /* the KEY_ENTER in ncurses doesn't seem to be working */


struct mpd_connection_info {
    struct mpd_connection *connection;
    struct mpd_status *status;
    char *host;
    int port;
    int timeout;
} mpd_info;


int main() {

    /* Establish a connection with the local MPD server */
    mpd_info.connection = mpd_connection_new(MPD_DEFAULT_HOST,
                                                         MPD_DEFAULT_PORT,
                                                         MPD_DEFAULT_TIMEOUT);

    if (mpd_connection_get_error(mpd_info.connection) != MPD_ERROR_SUCCESS) {
        printf("Error connecting to MPD server\n");
        mpd_connection_free(mpd_info.connection);
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

    struct title_bar *title_bar = title_bar_init("Queue", mpd_info.connection);
    struct queue_window *queue_window = queue_window_init();
    struct status_bar *status_bar = status_bar_init(mpd_info.connection);

    title_bar_draw(title_bar);

    queue_window_populate(queue_window, mpd_info.connection);
    queue_window_draw_all(queue_window);

    status_bar_draw(status_bar);

    wnoutrefresh(queue_window->win);
    wnoutrefresh(title_bar->win);
    wnoutrefresh(status_bar->win);

    int ch;
    while ((ch = getch()) != 'q') {

        title_bar_update_volume(title_bar, mpd_info.connection);
        title_bar_draw(title_bar);

        status_bar_update(status_bar);
        status_bar_draw(status_bar);

        switch (ch) {
            case KEY_DOWN:
                queue_window_move_cursor(queue_window, DOWN);
                wnoutrefresh(queue_window->win);
                break;
            case KEY_UP:
                queue_window_move_cursor(queue_window, UP);
                wnoutrefresh(queue_window->win);
                break;
            case KEY_LEFT:
                mpd_run_change_volume(mpd_info.connection, -1);
                wnoutrefresh(title_bar->win);
                break;
            case KEY_RIGHT:
                mpd_run_change_volume(mpd_info.connection, 1);
                wnoutrefresh(title_bar->win);
                break;
            case KEY_NPAGE:
                queue_window_scroll_page(queue_window, DOWN);
                wnoutrefresh(queue_window->win);
                break;
            case KEY_PPAGE:
                queue_window_scroll_page(queue_window, UP);
                wnoutrefresh(queue_window->win);
                break;
            case KEY_RETURN:
                mpd_run_play_id(mpd_info.connection, mpd_song_get_id(queue_window->selected->song));
                break;
            case 'p':
                mpd_run_toggle_pause(mpd_info.connection);
                break;
            case 's':
                mpd_run_stop(mpd_info.connection);
                break;
        }
//        wnoutrefresh(status_bar->win);
        doupdate();
    }

    title_bar_free(title_bar);
    queue_window_free(queue_window);
    status_bar_free(status_bar);
    endwin();

    mpd_connection_free(mpd_info.connection);

    return 0;
}
