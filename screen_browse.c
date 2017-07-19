/******************************************************************************
 * screen_browse.c : screen for browsing the music library
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

#include "screen_browse.h"
#include <stdlib.h>

struct screen_browse *screen_browse_init()
{
    struct screen_browse *screen = malloc(sizeof(*screen));
    screen->list = list_init();

    return screen;
}

void screen_browse_free(struct screen_browse *screen)
{
    list_free(screen->list);
    screen->list = NULL;
    free(screen);
}
