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
#include "screen_browse.h"
#include "screen_help.h"
#include "title_bar.h"
#include "status_bar.h"
#include "command.h"
#include "command_player.h"

#include <panel.h>
#include <stdlib.h>
#include <locale.h>

#define NUM_PANELS 3

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
void global_cmd(command_t cmd, struct status_bar *status_bar);
void screen_cmd(command_t cmd, enum main_screen *visible_screen,
                PANEL **panels, struct title_bar *title_bar);

int main(int argc, char *argv[])
{
    mpd_setup(argv[1], argv[2], argv[3]);
    mpd_connection_info_update(mpd_info);

    /* Initialize the UI */
    ncurses_init();
    struct title_bar *title_bar = title_bar_init(screen_titles[QUEUE]);
    struct status_bar *status_bar = status_bar_init();
    struct screen_help *screen_help = screen_help_init();
    struct screen_queue *screen_queue = screen_queue_init();
    struct screen_browse *screen_browse = screen_browse_init();

    enum main_screen visible_screen = QUEUE;
    PANEL *screen_panels[NUM_PANELS];
    WINDOW *screen_wins[] = {
            screen_help->win,
            screen_queue->list->win,
            screen_browse->list->win
    };

    screen_panels[HELP] = new_panel(screen_wins[HELP]);
    screen_panels[QUEUE] = new_panel(screen_wins[QUEUE]);
    screen_panels[BROWSE] = new_panel(screen_wins[BROWSE]);
    update_panels();
    doupdate();

    title_bar_update_volume(title_bar);
    screen_queue_populate_list(screen_queue);
    screen_browse_populate(screen_browse);

    title_bar_draw(title_bar);
    status_bar_draw(status_bar);
    screen_help_draw(screen_help);
    screen_queue_draw(screen_queue);
    screen_browse_draw(screen_browse);

    int ch;
    command_t cmd;
    halfdelay(1); /* Dirty hack to prevent screen flickering. Will fix. */
    top_panel(screen_panels[QUEUE]);
    while (cmd != CMD_QUIT) {
        mpd_connection_info_update(mpd_info);
        title_bar_update_volume(title_bar);
        title_bar_draw(title_bar);
        status_bar_draw(status_bar);

        ch = getch();
        cmd = find_key_command(ch);

        switch(visible_screen) {
            case HELP:
                screen_help_cmd(cmd, screen_help);
                break;
            case QUEUE:
                screen_queue_update(screen_queue);
                screen_queue_cmd(cmd, screen_queue, status_bar);
                break;
            case BROWSE:
                screen_browse_cmd(cmd, screen_browse, status_bar);
                break;
        }
        global_cmd(cmd, status_bar);
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
    screen_browse_free(screen_browse);

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

void global_cmd(command_t cmd, struct status_bar *status_bar)
{
    switch(cmd) {
        unsigned rc;
        char * notification;
        case CMD_CROP:
            if (mpd_status_get_state(mpd_info->status) != MPD_STATE_PLAY)
                break;
            if (mpd_status_get_queue_length(mpd_info->status) <= 1)
                break;
            int current_song_pos = mpd_song_get_pos(mpd_info->current_song);
            mpd_run_delete_range(mpd_info->connection, 0, current_song_pos);
            mpd_run_delete_range(mpd_info->connection, 1, (unsigned)-1);
            break;
        case CMD_SHUFFLE:
            if (mpd_status_get_queue_length(mpd_info->status) == 0)
                break;
            mpd_run_shuffle(mpd_info->connection);
            status_bar_show_notification(status_bar, "Shuffled queue", 3);
            break;
        case CMD_DB_UPDATE:
            status_bar_show_notification(status_bar, "Database update started", 3);
            rc = mpd_run_update(mpd_info->connection, NULL);
            notification = (rc == 0) ? "Error updating music database" : "Database updated";
            status_bar_show_notification(status_bar, notification, 3);
            break;
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
        case CMD_SCREEN_BROWSE:
            *visible_screen = BROWSE;
            title_bar->title = screen_titles[BROWSE];
            top_panel(panels[*visible_screen]);
            break;
    }
}
