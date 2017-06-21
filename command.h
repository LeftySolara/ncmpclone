/******************************************************************************
 * command.h : interface for user input commands
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

#ifndef NCMPCLONE_COMMAND_H
#define NCMPCLONE_COMMAND_H

#define MAX_KEYS 3

typedef enum {
    CMD_NONE,
    CMD_PLAY,
    CMD_PAUSE,
    CMD_STOP,
    CMD_NEXT_SONG,
    CMD_PREV_SONG,
    CMD_RANDOM,
    CMD_SHUFFLE,
    CMD_CROP,
    CMD_VOL_UP,
    CMD_VOL_DOWN,
    CMD_LIST_MOVE_UP,
    CMD_LIST_MOVE_DOWN,
    CMD_LIST_PAGE_UP,
    CMD_LIST_PAGE_DOWN,
    CMD_CLEAR_QUEUE,
    CMD_SCREEN_HELP,
    CMD_SCREEN_QUEUE,
    CMD_QUIT
} command_t;

typedef struct {
    command_t cmd;
    int keys[MAX_KEYS];
    char *name;
    char *description;
} command_def_t;


command_t find_key_command(int key);
char *get_command_keys(command_t cmd);
char *get_command_desc(command_t cmd);

#endif //NCMPCLONE_COMMAND_H
