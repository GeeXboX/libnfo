/*
 * GeeXboX libnfo: a .NFO file reader/writer.
 * Copyright (C) 2009 Benjamin Zores <ben@geexbox.org>
 *
 * This file is part of libnfo.
 *
 * libnfo is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * libnfo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libvalhalla; if not, write to the Free Software
 * Foundation, Inc, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef NFO_H
#define NFO_H

/**
 * \file nfo.h
 *
 * GeeXboX libnfo public API header.
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define NFO_STRINGIFY(s) #s
#define NFO_TOSTRING(s) NFO_STRINGIFY(s)

#define NFO_VERSION_INT(a, b, c) (a << 16 | b << 8 | c)
#define NFO_VERSION_DOT(a, b, c) a ##.## b ##.## c
#define NFO_VERSION(a, b, c) NFO_VERSION_DOT(a, b, c)

#define LIBNFO_VERSION_MAJOR  0
#define LIBNFO_VERSION_MINOR  0
#define LIBNFO_VERSION_MICRO  1

#define LIBNFO_VERSION_INT NFO_VERSION_INT(LIBNFO_VERSION_MAJOR, \
                                               LIBNFO_VERSION_MINOR, \
                                               LIBNFO_VERSION_MICRO)
#define LIBNFO_VERSION     NFO_VERSION(LIBNFO_VERSION_MAJOR, \
                                           LIBNFO_VERSION_MINOR, \
                                           LIBNFO_VERSION_MICRO)
#define LIBNFO_VERSION_STR NFO_TOSTRING(LIBNFO_VERSION)
#define LIBNFO_BUILD       LIBNFO_VERSION_INT

#include <inttypes.h>

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* Structure definitions */
typedef struct nfo_stream_video_s nfo_stream_video_t;
typedef struct nfo_stream_audio_s nfo_stream_audio_t;
typedef struct nfo_stream_sub_s nfo_stream_sub_t;
typedef struct nfo_fileinfo_s nfo_fileinfo_t;
typedef struct nfo_actor_s nfo_actor_t;
typedef struct nfo_movie_s nfo_movie_t;
typedef struct nfo_tvshow_s nfo_tvshow_t;
typedef struct nfo_tvshow_episode_s nfo_tvshow_episode_t;
typedef enum nfo_field_s nfo_field_t;
typedef enum nfo_type_s nfo_type_t;

enum nfo_type_s
{
    NFO_MOVIE,
    NFO_TVSHOW,
    NFO_TVSHOW_EPISODE,
    NFO_ACTOR
};

enum nfo_field_s
{
   NFO_ACTOR_NAME,
   NFO_ACTOR_ROLE,
   NFO_TITLE,
   NFO_ORIGINAL_TITLE,
   NFO_RATING,
   NFO_YEAR,
   NFO_TOP250,
   NFO_VOTES,
   NFO_OUTLINE,
   NFO_PLOT,
   NFO_TAGLINE,
   NFO_RUNTIME,
   NFO_THUMB,
   NFO_MPAA,
   NFO_PLAYCOUNT,
   NFO_WATCHED,
   NFO_ID,
   NFO_FILE_PATH,
   NFO_TRAILER,
   NFO_GENRE,
   NFO_CREDITS,
   NFO_DIRECTOR,
   NFO_EPISODE_GUIDE_URL,
   NFO_SEASON,
   NFO_EPISODE,
   NFO_DISPLAYSEASON,
   NFO_DISPLAYEPISODE,
   NFO_TVDBID,
   NFO_PREMIERED,
   NFO_STUDIO,
   NFO_SORTORDER,
   NFO_LANGUAGE,
   NFO_EPISODE_ACTOR_SOURCE,
   NFO_AIRED
};

/* Get Methods */
void nfo_actor_get(nfo_actor_t *nfo_actor, nfo_field_t type, char *value);
void nfo_get(nfo_type_t nfo_type, nfo_field_t type, char *value);
void nfo_get_actors(nfo_type_t nfo_type, nfo_actor_t *value);

/* Set Methods */
void nfo_actor_set(nfo_actor_t *nfo_actor, nfo_field_t type, char *value);
void nfo_set(nfo_type_t nfo_type, nfo_field_t type, char *value);
void nfo_set_actors(nfo_type_t nfo_type, nfo_actor_t *value);

void nfo_tvshow_add_episode();
void nfo_tvshow_remove_episode();

void nfo_tvshow_add_actor();
void nfo_tvshow_remove_actor();

#endif /* NFO_H */
