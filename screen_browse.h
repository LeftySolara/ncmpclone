/******************************************************************************
 * screen_browse.h : screen for browsing the music library
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

#ifndef NCMPCLONE_SCREEN_BROWSE_H
#define NCMPCLONE_SCREEN_BROWSE_H

#include "list.h"

struct screen_browse {
    struct list *list;
};

struct screen_browse *screen_browse_init();
void screen_browse_free(struct screen_browse *screen);

void screen_browse_populate(struct screen_browse *screen);

static inline void screen_browse_draw(struct screen_browse *screen) {
    list_draw(screen->list);
}

#endif //NCMPCLONE_SCREEN_BROWSE_H
