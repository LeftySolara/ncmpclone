/******************************************************************************
 * command_player.c : user commands for the mpd player
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

#include "command_player.h"
#include "mpd_info.h"

void player_cmd(command_t cmd, struct status_bar *status_bar)
{
    switch(cmd) {
        case CMD_NONE:
            break;
        case CMD_PAUSE:
            mpd_run_toggle_pause(mpd_info->connection);
            break;
        case CMD_STOP:
            mpd_run_stop(mpd_info->connection);
            break;
        case CMD_NEXT_SONG:
            if (mpd_status_get_state(mpd_info->status) == MPD_STATE_STOP)
                status_bar_show_notification(status_bar, "Not playing", 3);
            else
                mpd_run_next(mpd_info->connection);
            break;
        case CMD_PREV_SONG:
            if (mpd_status_get_state(mpd_info->status) == MPD_STATE_STOP)
                status_bar_show_notification(status_bar, "Not playing", 3);
            else
                mpd_run_previous(mpd_info->connection);
            break;
        case CMD_SEEK_FORWARD:
            if (mpd_status_get_state(mpd_info->status) == MPD_STATE_PLAY
                    && mpd_status_get_elapsed_time(mpd_info->status)
                    != mpd_status_get_total_time(mpd_info->status)) {
                mpd_run_seek_pos(mpd_info->connection,
                                 mpd_song_get_pos(mpd_info->current_song),
                                 mpd_status_get_elapsed_time(mpd_info->status) + 1);
            }
            break;
        case CMD_SEEK_BACKWARD:
            if (mpd_status_get_state(mpd_info->status) == MPD_STATE_PLAY
                    && mpd_status_get_elapsed_time(mpd_info->status) > 0) {
                mpd_run_seek_pos(mpd_info->connection,
                                 mpd_song_get_pos(mpd_info->current_song),
                                 mpd_status_get_elapsed_time(mpd_info->status) - 1);
            }
            break;
        case CMD_RANDOM:
            mpd_run_random(mpd_info->connection, !mpd_status_get_random(mpd_info->status));
            char *notification;
            notification = !mpd_status_get_random(mpd_info->status) ?
                                       "Random mode is on" : "Random mode is off";
            status_bar_show_notification(status_bar, notification, 3);
            break;
        case CMD_VOL_UP:
            mpd_run_change_volume(mpd_info->connection, 1);
            break;
        case CMD_VOL_DOWN:
            mpd_run_change_volume(mpd_info->connection, -1);

    }
}
