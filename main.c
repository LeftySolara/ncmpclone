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
#include "screen_help.h"
#include "title_bar.h"
#include "status_bar.h"

#include <panel.h>
#include <stdlib.h>
#include <locale.h>

#define KEY_RETURN 10 /* KEY_ENTER in ncurses doesn't seem to be working */
#define NUM_PANELS 2


enum mpd_error mpd_err;
enum main_screen {HELP, QUEUE, BROWSE, ARTIST, SEARCH, LYRICS, OUTPUTS};

struct mpd_connection_info *mpd_info;
char *screen_titles[] = {
        "Help",
        "Queue",
        "Browse:",
        "All artists",
        "Search:",
        "Lyrics",
        "Outputs"
};


void ncurses_init()
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, 1);
    keypad(stdscr, TRUE);
    refresh();
}

/* Attempt to connect to MPD. Defaults to the local server. */
void mpd_setup(char *host, char *port, char *timeout)
{
    mpd_info = mpd_connection_info_init(host, port, timeout);
    mpd_err = mpd_make_connection(mpd_info);
    if (mpd_err != MPD_ERROR_SUCCESS) {
        printf("%s\n", mpd_connection_get_error_message(mpd_info->connection));
        mpd_connection_info_free(mpd_info);
        exit(mpd_err);
    }
}

int main(int argc, char *argv[])
{
    mpd_setup(argv[1], argv[2], argv[3]);
    ncurses_init();

    /* Initialize the UI */
    struct title_bar *title_bar = title_bar_init(screen_titles[QUEUE]);
    struct status_bar *status_bar = status_bar_init();
    struct screen_help *screen_help = screen_help_init();
    struct screen_queue *screen_queue = screen_queue_init();

    PANEL *visible_screen;
    PANEL *screen_panels[2];
    WINDOW *screen_wins[] = {
            screen_help->win,
            screen_queue->list->win
    };

    screen_panels[HELP] = new_panel(screen_wins[HELP]);
    screen_panels[QUEUE] = new_panel(screen_wins[QUEUE]);
    update_panels();
    doupdate();

    mpd_connection_info_update(mpd_info);
    mpd_connection_info_get_queue(mpd_info);
    title_bar_update_volume(title_bar);
    screen_queue_populate_list(screen_queue);

    title_bar_draw(title_bar);
    status_bar_draw(status_bar);
    screen_help_draw(screen_help);
    screen_queue_draw(screen_queue);

    wnoutrefresh(title_bar->win);
    wnoutrefresh(status_bar->win);
    wnoutrefresh(screen_queue->list->win);


    int ch;
    halfdelay(1); /* Dirty hack to prevent screen flickering. Will fix. */
    while ((ch = getch()) != 'q') {
        mpd_connection_info_update(mpd_info);
        title_bar_update_volume(title_bar);
        title_bar_draw(title_bar);
        status_bar_draw(status_bar);

        switch (ch) {
            case '1':
                visible_screen = screen_panels[HELP];
                title_bar->title = screen_titles[HELP];
                top_panel(visible_screen);
                break;
            case '2':
                visible_screen = screen_panels[QUEUE];
                title_bar->title = screen_titles[QUEUE];
                top_panel(visible_screen);
                break;
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
                mpd_run_play_pos(mpd_info->connection, screen_queue->list->selected_index);
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
        update_panels();
        wnoutrefresh(status_bar->win);
        wnoutrefresh(title_bar->win);
        doupdate();
    }

    for (int i = 0; i < NUM_PANELS; ++i)
        del_panel(screen_panels[i]);

    title_bar_free(title_bar);
    status_bar_free(status_bar);
    screen_help_free(screen_help);
    screen_queue_free(screen_queue);

    endwin();
    mpd_connection_info_free(mpd_info);

    return 0;
}
