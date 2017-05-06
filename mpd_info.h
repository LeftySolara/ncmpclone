/******************************************************************************
 * mpd_info.h : Information for the current MPD connection
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

#ifndef NCMPCLONE_MPD_INFO_H
#define NCMPCLONE_MPD_INFO_H

#include <mpd/client.h>

extern struct mpd_connection_info {
    struct mpd_connection *connection;
    struct mpd_status *status;
    char *host;
    unsigned int port;
    unsigned int timeout;
} mpd_info;

struct mpd_connection_info *mpd_connection_info_init();
void mpd_connection_info_free(struct mpd_connection_info *mpd_info);
enum mpd_error mpd_make_connection(struct mpd_connection_info *mpd_info);

#endif //NCMPCLONE_MPD_INFO_H
