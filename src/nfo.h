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

#define LIBNFO_VERSION_MAJOR  1
#define LIBNFO_VERSION_MINOR  0
#define LIBNFO_VERSION_MICRO  0

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
typedef struct nfo_s nfo_t;

typedef enum nfo_type_s
{
  NFO_UNKNOWN,
  NFO_MOVIE,
  NFO_TVSHOW,
  NFO_MUSIC,
} nfo_type_t;

typedef enum nfo_actor_field_s {
  NFO_ACTOR_NAME,
  NFO_ACTOR_ROLE,
  NFO_ACTOR_THUMB,
} nfo_actor_field_t;

typedef enum nfo_video_field_s {
  NFO_VIDEO_WIDTH,
  NFO_VIDEO_HEIGHT,
  NFO_VIDEO_CODEC,
  NFO_VIDEO_FORMAT_INFO,
  NFO_VIDEO_DURATION,
  NFO_VIDEO_BITRATE,
  NFO_VIDEO_BITRATE_MODE,
  NFO_VIDEO_BITRATE_MAX,
  NFO_VIDEO_CONTAINER,
  NFO_VIDEO_CODEC_ID,
  NFO_VIDEO_CODEC_INFO,
  NFO_VIDEO_SCAN_TYPE,
  NFO_VIDEO_ASPECT,
} nfo_video_field_t;

typedef enum nfo_audio_field_s {
  NFO_AUDIO_LANG,
  NFO_AUDIO_CODEC,
  NFO_AUDIO_CHANNELS,
  NFO_AUDIO_BITRATE,
} nfo_audio_field_t;

typedef enum nfo_sub_field_s {
  NFO_SUB_LANG,
} nfo_sub_field_t;

typedef enum nfo_movie_field_s {
  NFO_MOVIE_TITLE,
  NFO_MOVIE_ORIGINAL_TITLE,
  NFO_MOVIE_RATING,
  NFO_MOVIE_YEAR,
  NFO_MOVIE_TOP250,
  NFO_MOVIE_VOTES,
  NFO_MOVIE_OUTLINE,
  NFO_MOVIE_PLOT,
  NFO_MOVIE_TAGLINE,
  NFO_MOVIE_RUNTIME,
  NFO_MOVIE_THUMB,
  NFO_MOVIE_FAN_ART,
  NFO_MOVIE_MPAA,
  NFO_MOVIE_PLAY_COUNT,
  NFO_MOVIE_WATCHED,
  NFO_MOVIE_ID,
  NFO_MOVIE_TRAILER,
  NFO_MOVIE_GENRE,
  NFO_MOVIE_CREDITS,
  NFO_MOVIE_DIRECTOR,
  NFO_MOVIE_STUDIO,
} nfo_movie_field_t;

typedef enum nfo_tvshow_field_s {
  NFO_TVSHOW_TITLE,
  NFO_TVSHOW_RATING,
  NFO_TVSHOW_SEASON,
  NFO_TVSHOW_EPISODE,
  NFO_TVSHOW_DISPLAY_SEASON,
  NFO_TVSHOW_DISPLAY_EPISODE,
  NFO_TVSHOW_EPISODE_GUIDE_URL,
  NFO_TVSHOW_PLOT,
  NFO_TVSHOW_MPAA,
  NFO_TVSHOW_WATCHED,
  NFO_TVSHOW_GENRE,
  NFO_TVSHOW_PREMIERED,
  NFO_TVSHOW_STUDIO,
  NFO_TVSHOW_FANART,
  NFO_TVSHOW_FANART_HEADER,
  NFO_TVSHOW_FANART_COVER,
} nfo_tvshow_field_t;

typedef enum nfo_tvshow_episode_field_s {
  NFO_TVSHOW_EPISODE_TITLE,
  NFO_TVSHOW_EPISODE_RATING,
  NFO_TVSHOW_EPISODE_SEASON,
  NFO_TVSHOW_EPISODE_EPISODE,
  NFO_TVSHOW_EPISODE_PLOT,
  NFO_TVSHOW_EPISODE_THUMB,
  NFO_TVSHOW_EPISODE_FANART,
  NFO_TVSHOW_EPISODE_FANART_SEASON,
  NFO_TVSHOW_EPISODE_PLAY_COUNT,
  NFO_TVSHOW_EPISODE_CREDITS,
  NFO_TVSHOW_EPISODE_DIRECTOR,
  NFO_TVSHOW_EPISODE_AIRED,
  NFO_TVSHOW_EPISODE_VOTES,
} nfo_tvshow_episode_field_t;

/* (Un)Init Routines */
nfo_t *nfo_init (const char *filename);
void nfo_free (nfo_t *nfo);

/* NFO Get Methods */
nfo_type_t nfo_get_type (nfo_t *nfo);
nfo_movie_t *nfo_get_movie (nfo_t *nfo);
nfo_tvshow_episode_t *nfo_get_tvshow_episode (nfo_t *nfo);

/* Movie Get Methods */
char *nfo_movie_get (nfo_movie_t *movie, nfo_movie_field_t f);

int nfo_movie_get_actors_count (nfo_movie_t *movie);
nfo_actor_t *nfo_movie_get_actor (nfo_movie_t *movie, int id);

int nfo_movie_get_video_streams_count (nfo_movie_t *movie);
nfo_stream_video_t *nfo_movie_get_video_stream (nfo_movie_t *movie, int id);

int nfo_movie_get_audio_streams_count (nfo_movie_t *movie);
nfo_stream_audio_t *nfo_movie_get_audio_stream (nfo_movie_t *movie, int id);

int nfo_movie_get_sub_streams_count (nfo_movie_t *movie);
nfo_stream_sub_t *nfo_movie_get_sub_stream (nfo_movie_t *movie, int id);

/* TVShow Get Methods */
char *nfo_tvshow_episode_get (nfo_tvshow_episode_t *ep,
                              nfo_tvshow_episode_field_t f);

int nfo_tvshow_episode_get_actors_count (nfo_tvshow_episode_t *ep);
nfo_actor_t *nfo_tvshow_episode_get_actor (nfo_tvshow_episode_t *ep, int id);

int nfo_tvshow_episode_get_video_streams_count (nfo_tvshow_episode_t *ep);
nfo_stream_video_t *nfo_tvshow_episode_get_video_stream
 (nfo_tvshow_episode_t *ep, int id);

int nfo_tvshow_episode_get_audio_streams_count (nfo_tvshow_episode_t *ep);
nfo_stream_audio_t *nfo_tvshow_episode_get_audio_stream
 (nfo_tvshow_episode_t *ep, int id);

int nfo_tvshow_episode_get_sub_streams_count (nfo_tvshow_episode_t *ep);
nfo_stream_sub_t *nfo_tvshow_episode_get_sub_stream
 (nfo_tvshow_episode_t *ep, int id);

nfo_tvshow_t *nfo_tvshow_episode_get_show (nfo_tvshow_episode_t *ep);
char *nfo_tvshow_get (nfo_tvshow_t *ep, nfo_tvshow_field_t f);

/* Actor Get Methods */
char *nfo_actor_get (nfo_actor_t *actor, nfo_actor_field_t f);

/* Streams Get Methods */
char *nfo_video_stream_get (nfo_stream_video_t *video, nfo_video_field_t f);
char *nfo_audio_stream_get (nfo_stream_audio_t *audio, nfo_audio_field_t f);
char *nfo_sub_stream_get (nfo_stream_sub_t *sub, nfo_sub_field_t f);

#endif /* NFO_H */
