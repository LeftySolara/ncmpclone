/******************************************************************************
 * status_bar.c : display label and progress for currently playing track
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

#include "status_bar.h"
#include "mpd_info.h"
#include <stdlib.h>
#include <string.h>

struct status_bar *status_bar_init()
{
    struct status_bar *status_bar = malloc(sizeof(struct status_bar));
    status_bar->win =newwin(2, COLS, LINES - 2, 0);

    return status_bar;
}

void status_bar_free(struct status_bar *bar)
{
    delwin(bar->win);
}

void status_bar_draw(struct status_bar *bar)
{
    char *track_label = create_song_label(mpd_info->current_song);

    wclear(bar->win);
    for (int i = 0; i < COLS; ++i)
        waddch(bar->win, ACS_HLINE);

    if (mpd_info->current_song == NULL)
        return;

    int state = mpd_status_get_state(mpd_info->status);
    char *state_label;

    if (state == MPD_STATE_PLAY)
        state_label = "Playing:";
    else if (state == MPD_STATE_PAUSE)
        state_label = "[Paused]";
    else
        state_label = " ";

    if (state != MPD_STATE_UNKNOWN && state != MPD_STATE_STOP) {
        wattr_on(bar->win, A_BOLD, NULL);
        mvwaddstr(bar->win, 1, 0, state_label);
        wattr_off(bar->win, A_BOLD, NULL);
        mvwaddstr(bar->win, 1, strlen(state_label) + 1, track_label);
    }

    free(track_label);
}

// TODO: Move this function to a shared header (currently a copy-paste job)
/* Create a song label of the format "artist - title" */
char *create_song_label(struct mpd_song *song)
{
    const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
    const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
    char *buffer = malloc(strlen(artist) + strlen(title) + 4);

    if (buffer == NULL) {
        printf("Cannot create track label. No memory available.");
    }
    else {
        strcpy(buffer, artist);
        strcat(buffer, " - ");
        strcat(buffer, title);
    }

    return buffer;
}
