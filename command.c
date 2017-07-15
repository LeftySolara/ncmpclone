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
#include "ncmpclone_ncurses.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define KEY_CTRL(x) ((x) & 0x1f)
#define KEY_RETURN 10 /* KEY_ENTER in ncurses doesn't seem to be working */

static command_def_t cmds[] = {

        {CMD_NONE, {0, 0, 0}, "None", "Null command"},

        {CMD_PLAY, {KEY_RETURN, 0, 0,},"Play",
        "Play"},

        {CMD_PAUSE, {'P', 0, 0}, "Pause",
        "Pause"},

        {CMD_STOP, {'s', KEY_BACKSPACE, 0}, "Stop",
        "Stop"},

        {CMD_SEEK_FORWARD, {'f', 0 ,0}, "Seek forward",
        "Seek forward"},

        {CMD_SEEK_BACKWARD, {'b', 0, 0}, "Seek backward",
        "Seek backward"},

        {CMD_NEXT_SONG, {'>', 0, 0}, "Next song",
        "Next song"},

        {CMD_PREV_SONG, {'<', 0, 0}, "Previous song",
        "Previous song"},

        {CMD_RANDOM, {'z', 0, 0}, "Random",
        "Toggle random mode"},

        {CMD_REPEAT, {'r', 0, 0}, "Repeat",
        "Toggle repeat mode"},

        {CMD_SINGLE, {'y', 0, 0}, "Single",
        "Toggle single mode"},

        {CMD_CONSUME, {'C', 0, 0}, "Consume",
        "Toggle consume mode"},

        {CMD_CROSSFADE, {'x', 0, 0}, "Crossfade",
        "Toggle crossfade mode"},

        {CMD_DB_UPDATE, {KEY_CTRL('u'), 0, 0}, "Update",
        "Start a music database update"},

        {CMD_SHUFFLE, {'Z', 0, 0}, "Shuffle",
        "Shuffle queue"},

        {CMD_CROP, {'o', 0, 0}, "Crop",
        "Crop"},

        {CMD_VOL_UP, {'+', KEY_RIGHT, 0}, "Volume Up",
        "Increase volume"},

        {CMD_VOL_DOWN, {'-', KEY_LEFT, 0}, "Volume down",
        "Decrease volume"},

        {CMD_LIST_MOVE_UP, {'k', KEY_UP, 0}, "Move up",
        "Move cursor up"},

        {CMD_LIST_MOVE_DOWN, {'j', KEY_DOWN, 0}, "Move down",
        "Move cursor down"},

        {CMD_LIST_MOVE_TOP, {KEY_HOME, KEY_CTRL('a'), 0}, "Move to list top",
        "Move cursor to the top of the list"},

        {CMD_LIST_MOVE_BOTTOM, {KEY_END, KEY_CTRL('e'), 0}, "Move to list bottom",
        "Move cursor to the bottom of the list"},

        {CMD_LIST_MOVE_SCREEN_TOP, {'H', 0, 0}, "Move to top",
        "Move cursor to the top of the screen"},

        {CMD_LIST_MOVE_SCREEN_MID, {'M', 0, 0}, "Move to middle",
        "Move cursor to the middle of the screen"},

        {CMD_LIST_MOVE_SCREEN_BOTTOM, {'L', 0, 0}, "Move to bottom",
        "Move cursor to the bottom of the screen"},

        {CMD_LIST_SCROLL_LINE_UP, {KEY_CTRL('b'), 0, 0}, "Scroll line up",
        "Scroll up one line"},

        {CMD_LIST_SCROLL_LINE_DOWN, {KEY_CTRL('n'), 0, 0}, "Scroll line down",
        "Scroll down one line"},

        {CMD_LIST_SCROLL_HALF_UP, {'N', 0, 0}, "Scroll up half a screen",
        "Scroll up half a screen"},

        {CMD_LIST_SCROLL_HALF_DOWN, {'B', 0, 0}, "Scroll down half a screen",
        "Scroll down half a screen"},

        {CMD_LIST_PAGE_UP, {KEY_PPAGE, 0, 0}, "Page up",
        "Page up"},

        {CMD_LIST_PAGE_DOWN, {KEY_NPAGE, 0, 0}, "Page down",
        "Page down"},

        {CMD_LIST_TOGGLE_RANGE_SELECT, {'v', 0, 0}, "Range selection",
        "Range selection"},

        {CMD_CLEAR_QUEUE, {'c', 0, 0}, "Clear Queue",
        "Clear queue"},

        {CMD_SCREEN_HELP, {'1', KEY_F(1), 'h'}, "Help",
        "Help screen"},

        {CMD_SCREEN_QUEUE, {'2', KEY_F(2), 0}, "Queue",
        "Queue screen"},

        {CMD_QUIT, {'q', 'Q', KEY_CTRL('c')}, "Quit",
        "Quit"}
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

char *get_command_keys(command_t cmd)
{
    int *keys = cmds[cmd].keys;
    int buf_size = 20;
    char *buffer = calloc(buf_size, sizeof(char));

    if (buffer == NULL)
        return "";

    strcpy(buffer, key_to_str(keys[0]));

    if (keys[1] != 0) {
        strcat(buffer, " ");
        strcat(buffer, key_to_str(keys[1]));
    }
    if (keys[2] != 0) {
        strcat(buffer, " ");
        strcat(buffer, key_to_str(keys[2]));
    }

    return buffer;
}

char *get_command_desc(command_t cmd)
{
    return cmds[cmd].description;
}
