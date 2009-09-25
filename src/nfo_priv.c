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

#include <stdlib.h>

#include "nfo.h"
#include "nfo_priv.h"

static inline void
list_free (void *field, void (*fct) (void *p))
{
  void **l = NULL;

  *l = field;
  while (*l)
  {
    fct (*l);
    (void) *l++;
  }
}

/* Actors */

nfo_actor_t *
nfo_actor_new (void)
{
  nfo_actor_t *actor;

  actor = calloc (1, sizeof (nfo_actor_t));
  return actor;
}

void
nfo_actor_free (void *p)
{
  nfo_actor_t *n = p;

  if (!n)
    return;

  NFREE (n->name);
  NFREE (n->role);
  NFREE (n->thumb);
  free (n);
}

/* Video Stream */

nfo_stream_video_t *
nfo_stream_video_new (void)
{
  nfo_stream_video_t *n;

  n = calloc (1, sizeof (nfo_stream_video_t));
  return n;
}

void
nfo_stream_video_free (void *p)
{
  nfo_stream_video_t *n = p;

  if (!n)
    return;

  NFREE (n->width);
  NFREE (n->height);
  NFREE (n->codec);
  NFREE (n->format_info);
  NFREE (n->duration);
  NFREE (n->bitrate);
  NFREE (n->bitrate_mode);
  NFREE (n->bitrate_max);
  NFREE (n->container);
  NFREE (n->codec_id);
  NFREE (n->codec_info);
  NFREE (n->scan_type);
  NFREE (n->aspect);
  free (n);
}

/* Audio Stream */

nfo_stream_audio_t *
nfo_stream_audio_new (void)
{
  nfo_stream_audio_t *n;

  n = calloc (1, sizeof (nfo_stream_audio_t));
  return n;
}

void
nfo_stream_audio_free (void *p)
{
  nfo_stream_audio_t *n = p;

  if (!n)
    return;

  NFREE (n->lang);
  NFREE (n->codec);
  NFREE (n->channels);
  NFREE (n->bitrate);
  free (n);
}

/* Subtitle Stream */

nfo_stream_sub_t *
nfo_stream_sub_new (void)
{
  nfo_stream_sub_t *n;

  n = calloc (1, sizeof (nfo_stream_sub_t));
  return n;
}

void
nfo_stream_sub_free (void *p)
{
  nfo_stream_sub_t *n = p;

  if (!n)
    return;

  NFREE (n->lang);
  free (n);
}

/* FileInfo */

nfo_fileinfo_t *
nfo_fileinfo_new (void)
{
  nfo_fileinfo_t *n;

  n = calloc (1, sizeof (nfo_fileinfo_t));
  return n;
}

void
nfo_fileinfo_free (nfo_fileinfo_t *n)
{
  if (!n)
    return;

  list_free (n->videos, nfo_stream_video_free);
  list_free (n->audios, nfo_stream_audio_free);
  list_free (n->subs, nfo_stream_sub_free);
  free (n);
}

/* Movie */

nfo_movie_t *
nfo_movie_new (void)
{
  nfo_movie_t *n;

  n = calloc (1, sizeof (nfo_movie_t));
  return n;
}

void
nfo_movie_free (nfo_movie_t *n)
{
  if (!n)
    return;

  NFREE (n->title);
  NFREE (n->original_title);
  NFREE (n->rating);
  NFREE (n->year);
  NFREE (n->top250);
  NFREE (n->votes);
  NFREE (n->outline);
  NFREE (n->plot);
  NFREE (n->tagline);
  NFREE (n->runtime);
  NFREE (n->thumb);
  NFREE (n->fanart);
  NFREE (n->mpaa);
  NFREE (n->playcount);
  NFREE (n->watched);
  NFREE (n->id);
  NFREE (n->trailer);
  NFREE (n->genre);
  NFREE (n->credits);
  NFREE (n->director);
  NFREE (n->studio);

  list_free (n->actors, nfo_actor_free);
  free (n);
}

/* TVShow */

nfo_tvshow_t *
nfo_tvshow_new (void)
{
  nfo_tvshow_t *n;

  n = calloc (1, sizeof (nfo_tvshow_t));
  return n;
}

void
nfo_tvshow_free (nfo_tvshow_t *n)
{
  if (!n)
    return;

  NFREE (n->title);
  NFREE (n->rating);
  NFREE (n->season);
  NFREE (n->episode);
  NFREE (n->displayseason);
  NFREE (n->displayepisode);
  NFREE (n->episode_guide_url);
  NFREE (n->plot);
  NFREE (n->mpaa);
  NFREE (n->watched);
  NFREE (n->genre);
  NFREE (n->premiered);
  NFREE (n->studio);

  list_free (n->actors, nfo_actor_free);
  free (n);
}

/* TVShow Episode */

nfo_tvshow_episode_t *
nfo_tvshow_episode_new (void)
{
  nfo_tvshow_episode_t *n;

  n = calloc (1, sizeof (nfo_tvshow_episode_t));
  return n;
}

void
nfo_tvshow_episode_free (nfo_tvshow_episode_t *n)
{
  if (!n)
    return;

  NFREE (n->title);
  NFREE (n->rating);
  NFREE (n->season);
  NFREE (n->episode);
  NFREE (n->plot);
  NFREE (n->thumb);
  NFREE (n->playcount);
  NFREE (n->credits);
  NFREE (n->director);
  NFREE (n->aired);
  NFREE (n->votes);

  nfo_tvshow_free (n->show);
  list_free (n->actors, nfo_actor_free);
  free (n);
}
