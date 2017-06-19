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
    status_bar->notification = NULL;
    status_bar->notify_end = 0;

    return status_bar;
}

void status_bar_free(struct status_bar *bar)
{
    delwin(bar->win);
}

/* Draw a progress bar for the currently playing track */
void status_bar_draw_progress(struct status_bar *bar)
{
    if (mpd_status_get_state(mpd_info->status) == MPD_STATE_STOP)
        return;

    double song_length = mpd_song_get_duration(mpd_info->current_song);
    double time_elapsed = mpd_status_get_elapsed_time(mpd_info->status);
    double width = getmaxx(bar->win);

    double secs_per_tick = song_length / width;
    double tick_size = (width / song_length) + 1;
    double ticks_elapsed = time_elapsed / tick_size;

    whline(bar->win, '=', (tick_size * ticks_elapsed) / secs_per_tick);
    mvwaddch(bar->win, 0, (tick_size * ticks_elapsed) / secs_per_tick, 'O');
}

void status_bar_draw(struct status_bar *bar)
{
    wclear(bar->win);
    whline(bar->win, ACS_HLINE, getmaxx(bar->win));

    if (mpd_info->current_song == NULL)
        return;

    status_bar_draw_progress(bar);
    if (bar->notification && time(NULL) <= bar->notify_end) {
        wattr_on(bar->win, A_BOLD, NULL);
        mvwaddstr(bar->win, 1, 0, bar->notification);
        wattr_off(bar->win, A_BOLD, NULL);
        return;
    }

    char *track_label = create_song_label(mpd_info->current_song);
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

    bar->progress_label = create_progress_label(bar->progress_label);
    if (bar->progress_label)
        mvwaddstr(bar->win, 1, COLS - strlen(bar->progress_label), bar->progress_label);

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

/* Create and return a string with the progress of the currently playing song */
char *create_progress_label(char *buffer)
{
    int total_time = mpd_status_get_total_time(mpd_info->status);
    int elapsed_time = mpd_status_get_elapsed_time(mpd_info->status);

    if (total_time == 0) /* no song playing */
        return NULL;

    const size_t buf_size = strlen("[123:45/123:45]");
    buffer = calloc(buf_size, sizeof(char));

    int total_minutes = total_time / 60;
    int total_seconds = total_time % 60;
    int elapsed_minutes = elapsed_time / 60;
    int elapsed_seconds = elapsed_time % 60;

    snprintf(buffer, buf_size, "[%d:%02d/%d:%02d]",
             elapsed_minutes, elapsed_seconds, total_minutes, total_seconds);

    return buffer;
}
