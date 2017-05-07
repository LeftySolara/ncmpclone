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

#include "mpd_info.h"
#include "queue_window.h"
#include "title_bar.h"
#include "status_bar.h"

#include <menu.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define KEY_RETURN 10 /* the KEY_ENTER in ncurses doesn't seem to be working */

// TODO: Add optional command-line arguments for MPD host and port
// TODO: Check for MPD_HOST and MPD_PORT environment variables before connecting

// TODO: Currently playing song is displaying in bold (correctly), but doesn't update without user input
// TODO: Fix status bar flickering

struct mpd_connection_info *mpd_info;

int main(int argc, char *argv[]) {

    mpd_info = mpd_connection_info_init();
    mpd_info->host = argv[1];
    mpd_info->port = atoi(argv[2]);
    mpd_info->timeout = atoi(argv[3]);

    enum mpd_error err = mpd_make_connection(mpd_info);
    if (err != MPD_ERROR_SUCCESS) {
        printf("%s\n", mpd_connection_get_error_message(mpd_info->connection));
        mpd_connection_info_free(mpd_info);
        exit(err);
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

    struct title_bar *title_bar = title_bar_init("Queue");
    struct queue_window *queue_window = queue_window_init();
    struct status_bar *status_bar = status_bar_init(mpd_info->connection);

    mpd_connection_info_update(mpd_info);

    title_bar_update_volume(title_bar);
    title_bar_draw(title_bar);

    queue_window_populate(queue_window);
    queue_window_draw_all(queue_window);

    status_bar_draw(status_bar);

    wnoutrefresh(queue_window->win);
    wnoutrefresh(title_bar->win);
    wnoutrefresh(status_bar->win);

    int ch;
    while ((ch = getch()) != 'q') {

        mpd_connection_info_update(mpd_info);

        title_bar_update_volume(title_bar);
        title_bar_draw(title_bar);

        status_bar_draw(status_bar);
        wnoutrefresh(status_bar->win);

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
                mpd_run_change_volume(mpd_info->connection, -1);
                wnoutrefresh(title_bar->win);
                break;
            case KEY_RIGHT:
                mpd_run_change_volume(mpd_info->connection, 1);
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
                mpd_run_play_id(mpd_info->connection, mpd_song_get_id(queue_window->selected->song));
                break;
            case 'p':
                mpd_run_toggle_pause(mpd_info->connection);
                break;
            case 's':
                mpd_run_stop(mpd_info->connection);
                break;
        }
        doupdate();
    }

    title_bar_free(title_bar);
    queue_window_free(queue_window);
    status_bar_free(status_bar);
    endwin();

    mpd_connection_info_free(mpd_info);

    return 0;
}
