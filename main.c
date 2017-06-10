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
#include "command.h"
#include "command_player.h"

#include <panel.h>
#include <stdlib.h>
#include <locale.h>

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

void ncurses_init();
void mpd_setup(char *host, char *port, char *timeout);
void screen_cmd(command_t cmd, enum main_screen *visible_screen,
                PANEL **panels, struct title_bar *title_bar);

int main(int argc, char *argv[])
{
    mpd_setup(argv[1], argv[2], argv[3]);
    ncurses_init();

    /* Initialize the UI */
    struct title_bar *title_bar = title_bar_init(screen_titles[QUEUE]);
    struct status_bar *status_bar = status_bar_init();
    struct screen_help *screen_help = screen_help_init();
    struct screen_queue *screen_queue = screen_queue_init();

    enum main_screen visible_screen = QUEUE;
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
    command_t cmd;
    halfdelay(1); /* Dirty hack to prevent screen flickering. Will fix. */
    while (cmd != CMD_QUIT) {
        mpd_connection_info_update(mpd_info);
        title_bar_update_volume(title_bar);
        title_bar_draw(title_bar);
        status_bar_draw(status_bar);

        ch = getch();
        cmd = find_key_command(ch);

        switch(visible_screen) {
            case QUEUE:
                screen_queue_cmd(cmd, screen_queue);
                break;
        }
        player_cmd(cmd, status_bar);
        screen_cmd(cmd, &visible_screen, screen_panels, title_bar);

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

void screen_cmd(command_t cmd, enum main_screen *visible_screen,
                PANEL **panels, struct title_bar *title_bar)
{
    switch(cmd) {
        case CMD_SCREEN_HELP:
            *visible_screen = HELP;
            title_bar->title = screen_titles[HELP];
            top_panel(panels[*visible_screen]);
            break;
        case CMD_SCREEN_QUEUE:
            *visible_screen = QUEUE;
            title_bar->title = screen_titles[QUEUE];
            top_panel(panels[*visible_screen]);
            break;
    }
}
