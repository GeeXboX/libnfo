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
#include "nfo_xml_parser.h"

static int
list_get_length (void *list)
{
  void **l = list;
  int n = 0;

  if (!list)
    return 0;

  while (*(l++))
    n++;

  return n;
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
nfo_actor_free (nfo_actor_t *n)
{
  if (!n)
    return;

  NFREE (n->name);
  NFREE (n->role);
  NFREE (n->thumb);
  free (n);
}

static void
nfo_actor_list_free (nfo_actor_t **actors)
{
  int n, i;

  if (!actors)
    return;

  n = list_get_length (actors) + 1;

  for (i = 0; i < n; i++)
    nfo_actor_free (actors[i]);
  free (actors);
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
nfo_stream_video_free (nfo_stream_video_t *n)
{
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

static void
nfo_stream_video_list_free (nfo_stream_video_t **videos)
{
  int n, i;

  if (!videos)
    return;

  n = list_get_length (videos) + 1;

  for (i = 0; i < n; i++)
    nfo_stream_video_free (videos[i]);
  free (videos);
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
nfo_stream_audio_free (nfo_stream_audio_t *n)
{
  if (!n)
    return;

  NFREE (n->lang);
  NFREE (n->codec);
  NFREE (n->channels);
  NFREE (n->bitrate);
  free (n);
}

static void
nfo_stream_audio_list_free (nfo_stream_audio_t **audios)
{
  int n, i;

  if (!audios)
    return;

  n = list_get_length (audios) + 1;

  for (i = 0; i < n; i++)
    nfo_stream_audio_free (audios[i]);
  free (audios);
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
nfo_stream_sub_free (nfo_stream_sub_t *n)
{
  if (!n)
    return;

  NFREE (n->lang);
  free (n);
}

static void
nfo_stream_sub_list_free (nfo_stream_sub_t **subs)
{
  int n, i;

  if (!subs)
    return;

  n = list_get_length (subs) + 1;

  for (i = 0; i < n; i++)
    nfo_stream_sub_free (subs[i]);
  free (subs);
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

  nfo_stream_video_list_free (n->videos);
  nfo_stream_audio_list_free (n->audios);
  nfo_stream_sub_list_free (n->subs);
  free (n);
}

void
nfo_fileinfo_add_stream_video (nfo_fileinfo_t *info, nfo_stream_video_t *v)
{
  int n;

  if (!info || !v)
    return;

  n = list_get_length ((void *) info->videos) + 1;
  info->videos = realloc (info->videos,
                          (n + 1) * sizeof (*(info->videos)));
  info->videos[n] = NULL;
  info->videos[n - 1] = v;
}

void
nfo_fileinfo_add_stream_audio (nfo_fileinfo_t *info, nfo_stream_audio_t *a)
{
  int n;

  if (!info || !a)
    return;

  n = list_get_length ((void *) info->audios) + 1;
  info->audios = realloc (info->audios,
                          (n + 1) * sizeof (*(info->audios)));
  info->audios[n] = NULL;
  info->audios[n - 1] = a;
}

void
nfo_fileinfo_add_stream_sub (nfo_fileinfo_t *info, nfo_stream_sub_t *s)
{
  int n;

  if (!info || !s)
    return;

  n = list_get_length ((void *) info->subs) + 1;
  info->subs = realloc (info->subs,
                        (n + 1) * sizeof (*(info->subs)));
  info->subs[n] = NULL;
  info->subs[n - 1] = s;
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

  nfo_fileinfo_free (n->fileinfo);
  nfo_actor_list_free (n->actors);
  free (n);
}

void
nfo_movie_add_actor (nfo_movie_t *movie, nfo_actor_t *actor)
{
  int n;

  if (!movie || !actor)
    return;

  n = list_get_length ((void *) movie->actors) + 1;
  movie->actors = realloc (movie->actors,
                           (n + 1) * sizeof (*(movie->actors)));
  movie->actors[n] = NULL;
  movie->actors[n - 1] = actor;
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

  nfo_actor_list_free (n->actors);
  free (n);
}

void
nfo_tvshow_add_actor (nfo_tvshow_t *tvshow, nfo_actor_t *actor)
{
  int n;

  if (!tvshow || !actor)
    return;

  n = list_get_length ((void *) tvshow->actors) + 1;
  tvshow->actors = realloc (tvshow->actors,
                            (n + 1) * sizeof (*(tvshow->actors)));
  tvshow->actors[n] = NULL;
  tvshow->actors[n - 1] = actor;
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

  nfo_fileinfo_free (n->fileinfo);
  nfo_tvshow_free (n->show);
  nfo_actor_list_free (n->actors);
  free (n);
}

void
nfo_tvshow_episode_add_actor (nfo_tvshow_episode_t *episode,
                              nfo_actor_t *actor)
{
  int n;

  if (!episode || !actor)
    return;

  n = list_get_length ((void *) episode->actors) + 1;
  episode->actors = realloc (episode->actors,
                             (n + 1) * sizeof (*(episode->actors)));
  episode->actors[n] = NULL;
  episode->actors[n - 1] = actor;
}

nfo_t *
nfo_init (const char *filename)
{
  nfo_t *n;

  n = calloc (1, sizeof (nfo_t));
  n->type = NFO_UNKNOWN;

  nfo_parse_xml (n, filename);

  return n;
}

void
nfo_free (nfo_t *nfo)
{
  if (!nfo)
    return;

  nfo_movie_free (nfo->movie);
  nfo_tvshow_episode_free (nfo->tvshow);
  free (nfo);
}
