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
#include "mpd_info.h"
#include "ncmpclone_ncurses.h"
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#define PATH_MAXLEN 4096

struct screen_browse *screen_browse_init()
{
    struct screen_browse *screen = malloc(sizeof(*screen));
    screen->browse_list = list_init();
    screen->current_path = malloc(PATH_MAXLEN);

    return screen;
}

void screen_browse_free(struct screen_browse *screen)
{
    list_free(screen->browse_list);
    screen->browse_list = NULL;
    free(screen->current_path);
    free(screen);
}

void screen_browse_populate_dir(struct screen_browse *screen, const char *path)
{
    struct mpd_song *song;
    struct mpd_directory *directory;
    struct mpd_playlist *plist;
    struct mpd_entity *entity;
    enum mpd_entity_type et;
    char *dir_str;
    char *display_str = calloc(500, sizeof(char));

    list_clear(screen->browse_list);
    if (path) {
        snprintf(screen->current_path, strlen(path) + 1, "%s", path);
        list_append_item(screen->browse_list, "[..]", " ", false);
        mpd_send_list_meta(mpd_info->connection, screen->current_path);
    }
    else {
        sprintf(screen->current_path, "");
        mpd_send_list_meta(mpd_info->connection, NULL);
    }

    while ((entity = mpd_recv_entity(mpd_info->connection))) {
        et = mpd_entity_get_type(entity);
        switch (et) {
            case MPD_ENTITY_TYPE_DIRECTORY:
                directory = mpd_entity_get_directory(entity);
                dir_str = basename(mpd_directory_get_path(directory));
                sprintf(display_str, "[%s]", dir_str);
                break;
            case MPD_ENTITY_TYPE_SONG:
                song = mpd_entity_get_song(entity);
                display_str = create_track_label(song);
                break;
            case MPD_ENTITY_TYPE_PLAYLIST:
                plist = mpd_entity_get_playlist(entity);
                display_str = mpd_playlist_get_path(plist);
                break;
        }
        list_append_item(screen->browse_list, display_str, " ", false);
    }

    mpd_response_finish(mpd_info->connection);
    if (display_str)
        free(display_str);
}

void screen_browse_select(struct screen_browse *screen)
{
    int len = strlen(screen->browse_list->selected->left_str) + 1;
    char *new_path = malloc(len);
    snprintf(new_path, len, "%s", screen->browse_list->selected->left_str);

    /* TODO: Remember previous position when switching directories */
    if (strcmp(new_path, "[..]") == 0) {
        /* Parent is the root directory */
        if (strrchr(screen->current_path, '/') == NULL)
            screen_browse_populate_dir(screen, NULL);
        else
            ; /* TODO: Implement this */
    }
    else if (new_path[0] == '[') {
        memmove(new_path, new_path + 1, strlen(new_path));
        new_path[strlen(new_path) - 1] = '\0';
//        strcat(screen->current_path, "/");
//        strcat(screen->current_path, new_path);
        screen_browse_populate_dir(screen, new_path);
    }
    else {
        ;
    }

    free(new_path);
}

void screen_browse_cmd(command_t cmd, struct screen_browse *screen,
                       struct status_bar *status_bar)
{
    switch (cmd) {
        case CMD_LIST_MOVE_UP:
            screen_browse_move_cursor(screen, UP);
            break;
        case CMD_LIST_MOVE_DOWN:
            screen_browse_move_cursor(screen, DOWN);
            break;
        case CMD_LIST_MOVE_TOP:
            screen_browse_move_pos(screen, TOP);
            break;
        case CMD_LIST_MOVE_BOTTOM:
            screen_browse_move_pos(screen, BOT);
            break;
        case CMD_LIST_MOVE_SCREEN_TOP:
            screen_browse_move_screen_pos(screen, TOP);
            break;
        case CMD_LIST_MOVE_SCREEN_MID:
            screen_browse_move_screen_pos(screen, MID);
            break;
        case CMD_LIST_MOVE_SCREEN_BOTTOM:
            screen_browse_move_screen_pos(screen, BOT);
            break;
        case CMD_LIST_SCROLL_LINE_UP:
            screen_browse_scroll_line(screen, UP);
            break;
        case CMD_LIST_SCROLL_LINE_DOWN:
            screen_browse_scroll_line(screen, DOWN);
            break;
        case CMD_LIST_SCROLL_HALF_UP:
            screen_browse_scroll_half_page(screen, UP);
            break;
        case CMD_LIST_SCROLL_HALF_DOWN:
            screen_browse_scroll_half_page(screen, DOWN);
            break;
        case CMD_LIST_PAGE_UP:
            screen_browse_scroll_page(screen, UP);
            break;
        case CMD_LIST_PAGE_DOWN:
            screen_browse_scroll_page(screen, DOWN);
            break;
        case CMD_LIST_TOGGLE_RANGE_SELECT:
            list_toggle_range(screen->browse_list);
            if (screen->browse_list->range_select)
                status_bar_show_notification(status_bar,
                                             "Range selection enabled", 3);
            else
                status_bar_show_notification(status_bar,
                                             "Range selection disabled", 3);
            break;
        case CMD_PLAY:
            screen_browse_select(screen);
            screen_browse_draw(screen);
            break;
    }
}
