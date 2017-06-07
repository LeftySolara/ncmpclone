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

struct queue_song *queue_song_init(struct mpd_song *song)
{
    struct queue_song *queue_song = malloc(sizeof(*queue_song));
    queue_song->song = song;
    queue_song->next = NULL;

    return queue_song;
}

void queue_song_free(struct queue_song *queue_song)
{
    mpd_song_free(queue_song->song);
    queue_song->song = NULL;
    queue_song->next = NULL;
    free(queue_song);
}

void queue_song_append(struct queue_song **head, struct queue_song *song)
{
    if (*head == NULL)
        *head = song;
    else {
        struct queue_song *current = *head;
        while (current->next)
            current = current->next;

        current->next = song;
    }
}

struct mpd_connection_info *mpd_connection_info_init(char *host, char *port, char *timeout)
{
    struct mpd_connection_info *info = malloc(sizeof(struct mpd_connection_info));
    info->connection = NULL;
    info->status = NULL;

    info->host = (host == NULL) ? "localhost" : host;
    info->port = (port == NULL) ? 6600 : atoi(port);
    info->timeout = (timeout == NULL) ? 30000 : atoi(timeout);

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

    struct queue_song *current = mpd_info->queue_head;
    while (mpd_info->queue_head) {
        mpd_info->queue_head = current->next;
        queue_song_free(current);
        current = mpd_info->queue_head;
    }
}

/* Query the MPD server for current information */
void mpd_connection_info_update(struct mpd_connection_info *mpd_info)
{
    mpd_info->status = mpd_run_status(mpd_info->connection);
    mpd_info->current_song = mpd_run_current_song(mpd_info->connection);
}

void mpd_connection_info_get_queue(struct mpd_connection_info *mpd_info)
{
    if (!mpd_info->connection)
        return;

    struct mpd_song *song;
    struct queue_song *queue_song;
    mpd_send_list_queue_meta(mpd_info->connection);

    while ((song = mpd_recv_song(mpd_info->connection))) {
        queue_song = queue_song_init(song);
        queue_song_append(&mpd_info->queue_head, queue_song);
    }
}

enum mpd_error mpd_make_connection(struct mpd_connection_info *mpd_info)
{
    mpd_info->connection = mpd_connection_new(mpd_info->host, mpd_info->port, mpd_info->timeout);
    enum mpd_error err = mpd_connection_get_error(mpd_info->connection);

    return err;
}
