/******************************************************************************
 * ncmpclone_ncurses.c : basic functions for interacting with ncurses
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

#include "ncmpclone_ncurses.h"
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define KEY_CTRL(x) ((x) & 0x1f)
#define KEY_RETURN 10

/* Create a song label of the format "artist - title" */
char *create_track_label(struct mpd_song *song)
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

/* Create a human-readable string representing a song's duration */
char *create_duration_label(struct mpd_song *song)
{
    const int buf_size = 7;
    char *buffer = calloc(buf_size, sizeof(char));

    if (buffer == NULL)
        printf("Cannot create duration string: not enough memory available.");
    else {
        int minutes = mpd_song_get_duration(song) / 60;
        int seconds = mpd_song_get_duration(song) % 60;
        snprintf(buffer, buf_size, "%d:%02d", minutes, seconds);
    }

    return buffer;
}

char *key_to_str(int key)
{
    char *str;
    switch(key) {
        case 0:
            str = NULL;
            break;
        case KEY_RETURN:
            str = "Enter";
            break;
        case KEY_BACKSPACE:
            str = "Backspace";
            break;
        case KEY_RIGHT:
            str = "Right";
            break;
        case KEY_LEFT:
            str = "Left";
            break;
        case KEY_UP:
            str = "Up";
            break;
        case KEY_DOWN:
            str = "Down";
            break;
        case KEY_PPAGE:
            str = "PageUp";
            break;
        case KEY_NPAGE:
            str = "PageDown";
            break;
        case KEY_HOME:
            str = "Home";
            break;
        case KEY_END:
            str = "End";
            break;
        case KEY_F(1):
            str = "F1";
            break;
        case KEY_F(2):
            str = "F2";
            break;
        case KEY_F(3):
            str = "F3";
            break;
        case KEY_F(4):
            str = "F4";
            break;
        case KEY_F(5):
            str = "F5";
            break;
        case KEY_F(6):
            str = "F6";
            break;
        case KEY_F(7):
            str = "F7";
            break;
        case KEY_F(8):
            str = "F8";
            break;
        case KEY_F(9):
            str = "F9";
            break;
        case KEY_F(10):
            str = "F10";
            break;
        case KEY_F(11):
            str = "F11";
            break;
        case KEY_F(12):
            str = "F12";
            break;
        default:
            if (!(key & ~0x1f)) {
                str = calloc(7, sizeof(char));
                snprintf(str, 7, "Ctrl-%c", 'A' + (key & 0x1f) - 1);
            }
            else
                str = (char *)&key;
    }
    return str;
}
