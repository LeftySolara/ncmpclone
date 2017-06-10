/******************************************************************************
 * command.c : interface for user input commands
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

#include "command.h"
#include <ncurses.h>

#define KEY_RETURN 10 /* KEY_ENTER in ncurses doesn't seem to be working */

static command_def_t cmds[] = {

        {CMD_NONE, {0, 0, 0}, "None", "Null command"},

        /* Player commands */
        {CMD_PLAY, {KEY_RETURN, 0, 0,},"Play",
        "Play the currently selected item"},

        {CMD_PAUSE, {'P', KEY_BACKSPACE, 0}, "Pause",
        "Pause the currently playing song"},

        {CMD_STOP, {'s', 0, 0}, "Stop",
        "Stop the currently playing song"},

        {CMD_NEXT_SONG, {'>', 0, 0}, "Next song",
        "Play the next song from the queue"},

        {CMD_PREV_SONG, {'<', 0, 0}, "Previous song",
        "Play the previous song from the queue"},

        {CMD_RANDOM, {'z', 0, 0}, "Random",
        "Toggle random mode"},

        {CMD_VOL_UP, {'+', KEY_RIGHT, 0}, "Volume Up",
        "Increase volume"},

        {CMD_VOL_DOWN, {'-', KEY_LEFT, 0}, "Volume down",
        "Decrease volume"},

        /* List window commands */
        {CMD_LIST_MOVE_UP, {'k', KEY_UP, 0}, "Move up",
        "Move cursor up"},

        {CMD_LIST_MOVE_DOWN, {'j', KEY_DOWN, 0}, "Move down",
        "Move cursor down"},

        {CMD_LIST_PAGE_UP, {KEY_PPAGE, 0, 0}, "Page up",
        "Page up"},

        {CMD_LIST_PAGE_DOWN, {KEY_NPAGE, 0, 0}, "Page down",
        "Page down"},

        /* Screen commands */
        {CMD_SCREEN_HELP, {'1', KEY_F(1), 'h'}, "Help",
        "Display the help screen"},

        {CMD_SCREEN_QUEUE, {'2', KEY_F(2), 0}, "Queue",
        "Display the queue screen"},

        {CMD_QUIT, {'q', 0, 0}, "Quit",
        "Exit ncmpclone"}
};

command_t find_key_command(int key)
{
    if (key == 0)
        return CMD_NONE;

    for (int i = 0; cmds[i].name; ++i) {
        for (int j = 0; j < MAX_KEYS; ++j) {
            if (key == cmds[i].keys[j])
                return cmds[i].cmd;
        }
    }

    return CMD_NONE;
}
