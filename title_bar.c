/******************************************************************************
 * title_bar.c : display screen title and current volume to the user
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

#include "title_bar.h"
#include "mpd_info.h"
#include <stdlib.h>
#include <string.h>

/* Create and initialize a title bar */
struct title_bar *title_bar_init(char *title)
{
    struct title_bar *title_bar = malloc(sizeof(struct title_bar));
    title_bar->win = newwin(2, COLS, 0, 0);
    title_bar->title = title;
    title_bar->width = COLS;

    return title_bar;
}

/* Free memory used by a title bar and its members */
void title_bar_free(struct title_bar *title_bar)
{
    delwin(title_bar->win);
    title_bar->win = NULL;
}

/* Query the MPD server for the current volume level and update the label */
void title_bar_update_volume(struct title_bar *title_bar)
{
    int volume = mpd_status_get_volume(mpd_info->status);
    snprintf(title_bar->volume_label, 12, "Volume %d%%", volume);
}

/* Print the title bar onto the screen */
void title_bar_draw(struct title_bar *title_bar)
{
    wclear(title_bar->win);
    wattr_on(title_bar->win, A_BOLD, NULL);
    mvwaddnstr(title_bar->win, 0, 0, title_bar->title, title_bar->width);
    wattr_off(title_bar->win, A_BOLD, NULL);
    mvwaddstr(title_bar->win, 0,
              title_bar->width - strlen(title_bar->volume_label),
              title_bar->volume_label);

    wmove(title_bar->win, 1, 0);
    whline(title_bar->win, ACS_HLINE, title_bar->width);

    if (mpd_status_get_random(mpd_info->status))
        mvwaddstr(title_bar->win, 1, COLS - strlen("[z]") - 1, "[z]");
}
