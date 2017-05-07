/******************************************************************************
 * mpd_info.c : Information for the current MPD connection
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

#include "mpd_info.h"
#include <stdlib.h>

struct mpd_connection_info *mpd_connection_info_init()
{
    struct mpd_connection_info *info = malloc(sizeof(struct mpd_connection_info));
    info->connection = NULL;
    info->status = NULL;
    info->host = "localhost";
    info->port = 6600;
    info->timeout = 30000;

    return info;
}

void mpd_connection_info_free(struct mpd_connection_info *mpd_info)
{
    if (mpd_info->connection)
        mpd_connection_free(mpd_info->connection);
    if (mpd_info->status)
        mpd_status_free(mpd_info->status);

    mpd_info->connection = NULL;
    mpd_info->status = NULL;
}

/* Query the MPD server for current information */
void mpd_connection_info_update(struct mpd_connection_info *mpd_info)
{
    mpd_info->status = mpd_run_status(mpd_info->connection);
    mpd_info->current_song = mpd_run_current_song(mpd_info->connection);
}

enum mpd_error mpd_make_connection(struct mpd_connection_info *mpd_info)
{
    mpd_info->connection = mpd_connection_new(mpd_info->host, mpd_info->port, mpd_info->timeout);
    enum mpd_error err = mpd_connection_get_error(mpd_info->connection);

    return err;
}
