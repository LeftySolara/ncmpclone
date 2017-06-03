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
#include "screen_queue.h"
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
enum Screen {HELP, QUEUE, BROWSE, ARTIST, SEARCH, LYRICS, OUTPUTS};

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
    struct screen_queue *screen_queue = screen_queue_init();
    struct status_bar *status_bar = status_bar_init();

    mpd_connection_info_update(mpd_info);

    title_bar_update_volume(title_bar);
    title_bar_draw(title_bar);

    screen_queue_populate(screen_queue);
    screen_queue_draw_all(screen_queue);

    status_bar_draw(status_bar);

    wnoutrefresh(screen_queue->win);
    wnoutrefresh(title_bar->win);
    wnoutrefresh(status_bar->win);

    int ch;
    enum Screen visible_screen = QUEUE;
    halfdelay(1);
    while ((ch = getch()) != 'q') {

        mpd_connection_info_update(mpd_info);

        title_bar_update_volume(title_bar);
        title_bar_draw(title_bar);

        status_bar_draw(status_bar);

        screen_queue_draw_all(screen_queue);

        switch (ch) {
            case KEY_DOWN:
                screen_queue_move_cursor(screen_queue, DOWN);
                break;
            case KEY_UP:
                screen_queue_move_cursor(screen_queue, UP);
                break;
            case KEY_LEFT:
                mpd_run_change_volume(mpd_info->connection, -1);
                break;
            case KEY_RIGHT:
                mpd_run_change_volume(mpd_info->connection, 1);
                break;
            case KEY_NPAGE:
                screen_queue_scroll_page(screen_queue, DOWN);
                break;
            case KEY_PPAGE:
                screen_queue_scroll_page(screen_queue, UP);
                break;
            case KEY_RETURN:
                mpd_run_play_id(mpd_info->connection, mpd_song_get_id(screen_queue->selected->song));
                break;
            case 'p':
                mpd_run_toggle_pause(mpd_info->connection);
                break;
            case 's':
                mpd_run_stop(mpd_info->connection);
                break;
            case '<':
                mpd_run_previous(mpd_info->connection);
                break;
            case '>':
                mpd_run_next(mpd_info->connection);
                break;
            case 'z':
                mpd_run_random(mpd_info->connection, !mpd_status_get_random(mpd_info->status));
                status_bar->notification = !mpd_status_get_random(mpd_info->status) ?
                                           "Random mode is on" : "Random mode is off";
                status_bar->notify_end = time(NULL) + 3;
                break;
        }

        switch (visible_screen) {
            case QUEUE:
                wnoutrefresh(screen_queue->win);
        }

        wnoutrefresh(status_bar->win);
        wnoutrefresh(title_bar->win);
        doupdate();
    }

    title_bar_free(title_bar);
    screen_queue_free(screen_queue);
    status_bar_free(status_bar);
    endwin();

    mpd_connection_info_free(mpd_info);

    return 0;
}
