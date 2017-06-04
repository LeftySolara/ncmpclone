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
    // For now we'll just print "Hello World" to make sure
    // we can switch between screens

    int x, y;
    getmaxyx(screen->win, y, x);
    mvwaddstr(screen->win, y / 2, x / 2, "Hello World!");
}
