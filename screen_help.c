/******************************************************************************
 * screen_help.c : screen for displaying application controls
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

#include "screen_help.h"
#include <stdlib.h>
#include <string.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))


/* We'll use CMD_NONE to print a blank line */
static command_t movement_cmds[] = {
        CMD_LIST_MOVE_UP,
        CMD_LIST_MOVE_DOWN,
        CMD_LIST_MOVE_TOP,
        CMD_LIST_MOVE_MID,
        CMD_LIST_MOVE_BOTTOM,
        CMD_LIST_PAGE_UP,
        CMD_LIST_PAGE_DOWN,
        CMD_NONE,
        CMD_SCREEN_HELP,
        CMD_SCREEN_QUEUE
};

static command_t global_cmds[] = {
        CMD_PAUSE,
        CMD_CROP,
        CMD_STOP,
        CMD_NEXT_SONG,
        CMD_PREV_SONG,
        CMD_RANDOM,
        CMD_VOL_UP,
        CMD_VOL_DOWN,
        CMD_NONE,
        CMD_QUIT
};

static command_t queue_screen_cmds[] = {
        CMD_PLAY,
        CMD_CLEAR_QUEUE
};

struct screen_help *screen_help_init()
{
    struct screen_help *screen = malloc(sizeof(*screen));
    screen->win = newwin(LINES - 4, COLS, 2, 0);
}

void screen_help_free(struct screen_help *screen)
{
    delwin(screen->win);
    free(screen);
}

void screen_help_draw(struct screen_help *screen)
{
    int y = 0;

    screen_help_draw_header(screen, y++, "Movement");
    for (int i = 0; i < NELEMS(movement_cmds); ++i)
        screen_help_draw_command(screen, ++y, movement_cmds[i]);

    y += 2;
    screen_help_draw_header(screen, y++, "Global");
    for (int i = 0; i < NELEMS(global_cmds); ++i)
        screen_help_draw_command(screen, ++y, global_cmds[i]);

    y+= 2;
    screen_help_draw_header(screen, y++, "Queue screen");
    for (int i = 0; i < NELEMS(queue_screen_cmds); ++i)
        screen_help_draw_command(screen, ++y, queue_screen_cmds[i]);
}

void screen_help_draw_header(struct screen_help *screen, const int begin_y, char *title)
{
    const int name_begin_x = 6;
    const int line_begin_x = 3;

    wattr_on(screen->win, A_BOLD, NULL);
    mvwaddstr(screen->win, begin_y, name_begin_x, title);
    wattr_off(screen->win, A_BOLD, NULL);

    wmove(screen->win, begin_y+1, line_begin_x);
    for (int i = line_begin_x; i < COLS - line_begin_x; ++i)
        waddch(screen->win, '-');
}

void screen_help_draw_command(struct screen_help *screen, const int begin_y, command_t cmd)
{
    if (cmd == CMD_NONE)
        return;
    const int colon_x_pos = 21;
    char *cmd_keys = get_command_keys(cmd);
    char *desc = get_command_desc(cmd);

    wmove(screen->win, begin_y, colon_x_pos - strlen(cmd_keys) - 1);
    waddstr(screen->win, cmd_keys);
    waddstr(screen->win, " : ");
    waddstr(screen->win, desc);
    free(cmd_keys);
}
