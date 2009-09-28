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

#ifndef NFO_PRIV_H
#define NFO_PRIV_H

#include <stdlib.h>

#include "nfo.h"
#include "nfo_priv.h"

#define NFREE(p) \
  { \
    if (p) { \
      free (p); \
      p = NULL; \
    } \
  }

struct nfo_actor_s {
  char *name;
  char *role;
  char *thumb;
};

nfo_actor_t *nfo_actor_new (void);
void nfo_actor_free (nfo_actor_t *n);

struct nfo_stream_video_s {
  char *width;
  char *height;
  char *codec;
  char *format_info;
  char *duration;
  char *bitrate;
  char *bitrate_mode;
  char *bitrate_max;
  char *container;
  char *codec_id;
  char *codec_info;
  char *scan_type;
  char *aspect;
};

nfo_stream_video_t *nfo_stream_video_new (void);
void nfo_stream_video_free (nfo_stream_video_t *n);

struct nfo_stream_audio_s {
  char *lang;
  char *codec;
  char *channels;
  char *bitrate;
};

nfo_stream_audio_t *nfo_stream_audio_new (void);
void nfo_stream_audio_free (nfo_stream_audio_t *n);

struct nfo_stream_sub_s {
  char *lang;
};

nfo_stream_sub_t *nfo_stream_sub_new (void);
void nfo_stream_sub_free (nfo_stream_sub_t *n);

struct nfo_fileinfo_s {
  nfo_stream_video_t **videos;
  nfo_stream_audio_t **audios;
  nfo_stream_sub_t   **subs;
};

nfo_fileinfo_t *nfo_fileinfo_new (void);
void nfo_fileinfo_free (nfo_fileinfo_t *n);

void nfo_fileinfo_add_stream_video (nfo_fileinfo_t *info,
                                    nfo_stream_video_t *v);
void nfo_fileinfo_add_stream_audio (nfo_fileinfo_t *info,
                                    nfo_stream_audio_t *a);
void nfo_fileinfo_add_stream_sub (nfo_fileinfo_t *info, nfo_stream_sub_t *s);

struct nfo_movie_s {
  char *title;
  char *original_title;
  char *rating;
  char *year;
  char *top250;
  char *votes;
  char *outline;
  char *plot;
  char *tagline;
  char *runtime;
  char *thumb;
  char *fanart;
  char *mpaa;
  char *playcount;
  char *watched;
  char *id;
  char *trailer;
  char *genre;
  char *credits;
  char *director;
  char *studio;
  nfo_fileinfo_t *fileinfo;
  nfo_actor_t **actors;
};

nfo_movie_t *nfo_movie_new (void);
void nfo_movie_free (nfo_movie_t *n);
void nfo_movie_add_actor (nfo_movie_t *movie, nfo_actor_t *actor);

struct nfo_tvshow_s {
  char *title;
  char *rating;
  char *season;
  char *episode;
  char *displayseason;
  char *displayepisode;
  char *episode_guide_url;
  char *plot;
  char *mpaa;
  char *watched;
  char *genre;
  char *premiered;
  char *studio;
  nfo_actor_t **actors;
};

nfo_tvshow_t *nfo_tvshow_new (void);
void nfo_tvshow_free (nfo_tvshow_t *n);
void nfo_tvshow_add_actor (nfo_tvshow_t *tvshow, nfo_actor_t *actor);

struct nfo_tvshow_episode_s {
  char *title;
  char *rating;
  char *season;
  char *episode;
  char *plot;
  char *thumb;
  char *fanart;
  char *fanart_season;
  char *playcount;
  char *credits;
  char *director;
  char *aired;
  char *votes;
  nfo_fileinfo_t *fileinfo;
  nfo_tvshow_t *show;
  nfo_actor_t **actors;
};

nfo_tvshow_episode_t *nfo_tvshow_episode_new (void);
void nfo_tvshow_episode_free (nfo_tvshow_episode_t *n);
void nfo_tvshow_episode_add_actor (nfo_tvshow_episode_t *episode,
                                   nfo_actor_t *actor);

struct nfo_s {
  nfo_type_t type;
  nfo_movie_t *movie;
  nfo_tvshow_episode_t *tvshow;
};

#endif /* NFO_PRIV_H */
