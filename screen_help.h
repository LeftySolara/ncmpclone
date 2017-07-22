/******************************************************************************
 * screen_help.h : screen for displaying application controls
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

#ifndef NCMPCLONE_SCREEN_HELP_H
#define NCMPCLONE_SCREEN_HELP_H

#include "command.h"
#include <ncurses.h>

struct screen_help {
    WINDOW *win;
    WINDOW *pad;
    int y_pos_top;
};

struct screen_help *screen_help_init();
void screen_help_free(struct screen_help *screen);
void screen_help_draw(struct screen_help *screen);
void screen_help_draw_header(struct screen_help *screen, const int begin_y, char *title);
void screen_help_draw_command(struct screen_help *screen, const int begin_y, command_t cmd);
void screen_help_cmd(command_t cmd, struct screen_help *screen);

#endif //NCMPCLONE_SCREEN_HELP_H
