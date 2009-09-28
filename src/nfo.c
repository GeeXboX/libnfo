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

nfo_type_t
nfo_get_type (nfo_t *nfo)
{
  return nfo ? nfo->type : NFO_UNKNOWN;
}

nfo_movie_t *
nfo_get_movie (nfo_t *nfo)
{
  return nfo ? nfo->movie : NULL;

}

nfo_tvshow_episode_t *
nfo_get_tvshow_episode (nfo_t *nfo)
{
  return nfo ? nfo->tvshow : NULL;
}

char *
nfo_movie_get (nfo_movie_t *movie, nfo_movie_field_t f)
{
  if (!movie)
    return NULL;

  switch (f)
  {
  case NFO_MOVIE_TITLE:
    return movie->title;
  case NFO_MOVIE_ORIGINAL_TITLE:
    return movie->original_title;
  case NFO_MOVIE_RATING:
    return movie->rating;
  case NFO_MOVIE_YEAR:
    return movie->year;
  case NFO_MOVIE_TOP250:
    return movie->top250;
  case NFO_MOVIE_VOTES:
    return movie->votes;
  case NFO_MOVIE_OUTLINE:
    return movie->outline;
  case NFO_MOVIE_PLOT:
    return movie->plot;
  case NFO_MOVIE_TAGLINE:
    return movie->tagline;
  case NFO_MOVIE_RUNTIME:
    return movie->runtime;
  case NFO_MOVIE_THUMB:
    return movie->thumb;
  case NFO_MOVIE_FAN_ART:
    return movie->fanart;
  case NFO_MOVIE_MPAA:
    return movie->mpaa;
  case NFO_MOVIE_PLAY_COUNT:
    return movie->playcount;
  case NFO_MOVIE_WATCHED:
    return movie->watched;
  case NFO_MOVIE_ID:
    return movie->id;
  case NFO_MOVIE_TRAILER:
    return movie->trailer;
  case NFO_MOVIE_GENRE:
    return movie->genre;
  case NFO_MOVIE_CREDITS:
    return movie->credits;
  case NFO_MOVIE_DIRECTOR:
    return movie->director;
  case NFO_MOVIE_STUDIO:
    return movie->studio;
  }

  return NULL;
}

int
nfo_movie_get_actors_count (nfo_movie_t *movie)
{
  return (movie && movie->actors) ? list_get_length (movie->actors) : 0;
}

nfo_actor_t *
nfo_movie_get_actor (nfo_movie_t *movie, int id)
{
  int max;

  if (!movie)
    return NULL;

  max = list_get_length (movie->actors);
  if (id >= max)
    return NULL;

  return movie->actors[id];
}

int
nfo_movie_get_video_streams_count (nfo_movie_t *movie)
{
  nfo_fileinfo_t *fileinfo;

  if (!movie || !movie->fileinfo)
    return 0;

  fileinfo = movie->fileinfo;
  return fileinfo->videos ? list_get_length (fileinfo->videos) : 0;
}

nfo_stream_video_t *
nfo_movie_get_video_stream (nfo_movie_t *movie, int id)
{
  nfo_fileinfo_t *fileinfo;
  int max;

  if (!movie || !movie->fileinfo)
    return NULL;

  fileinfo = movie->fileinfo;
  max = list_get_length (fileinfo->videos);
  if (id >= max)
    return NULL;

  return fileinfo->videos[id];
}

int
nfo_movie_get_audio_streams_count (nfo_movie_t *movie)
{
  nfo_fileinfo_t *fileinfo;

  if (!movie || !movie->fileinfo)
    return 0;

  fileinfo = movie->fileinfo;
  return fileinfo->audios ? list_get_length (fileinfo->audios) : 0;
}

nfo_stream_audio_t *
nfo_movie_get_audio_stream (nfo_movie_t *movie, int id)
{
  nfo_fileinfo_t *fileinfo;
  int max;

  if (!movie || !movie->fileinfo)
    return NULL;

  fileinfo = movie->fileinfo;
  max = list_get_length (fileinfo->audios);
  if (id >= max)
    return NULL;

  return fileinfo->audios[id];
}

int
nfo_movie_get_sub_streams_count (nfo_movie_t *movie)
{
  nfo_fileinfo_t *fileinfo;

  if (!movie || !movie->fileinfo)
    return 0;

  fileinfo = movie->fileinfo;
  return fileinfo->subs ? list_get_length (fileinfo->subs) : 0;
}

nfo_stream_sub_t *
nfo_movie_get_sub_stream (nfo_movie_t *movie, int id)
{
  nfo_fileinfo_t *fileinfo;
  int max;

  if (!movie || !movie->fileinfo)
    return NULL;

  fileinfo = movie->fileinfo;
  max = list_get_length (fileinfo->subs);
  if (id >= max)
    return NULL;

  return fileinfo->subs[id];
}

/* TVShow Get Methods */
char *
nfo_tvshow_episode_get (nfo_tvshow_episode_t *ep,
                        nfo_tvshow_episode_field_t f)
{
  if (!ep)
    return NULL;

  switch (f)
  {
  case NFO_TVSHOW_EPISODE_TITLE:
    return ep->title;
  case NFO_TVSHOW_EPISODE_RATING:
    return ep->rating;
  case NFO_TVSHOW_EPISODE_SEASON:
    return ep->season;
  case NFO_TVSHOW_EPISODE_EPISODE:
    return ep->episode;
  case NFO_TVSHOW_EPISODE_PLOT:
    return ep->plot;
  case NFO_TVSHOW_EPISODE_THUMB:
    return ep->thumb;
  case NFO_TVSHOW_EPISODE_FANART:
    return ep->fanart;
  case NFO_TVSHOW_EPISODE_FANART_SEASON:
    return ep->fanart_season;
  case NFO_TVSHOW_EPISODE_PLAY_COUNT:
    return ep->playcount;
  case NFO_TVSHOW_EPISODE_CREDITS:
    return ep->credits;
  case NFO_TVSHOW_EPISODE_DIRECTOR:
    return ep->director;
  case NFO_TVSHOW_EPISODE_AIRED:
    return ep->aired;
  case NFO_TVSHOW_EPISODE_VOTES:
    return ep->votes;
  }

  return NULL;
}

int
nfo_tvshow_episode_get_actors_count (nfo_tvshow_episode_t *ep)
{
  return (ep && ep->actors) ? list_get_length (ep->actors) : 0;
}

nfo_actor_t *
nfo_tvshow_episode_get_actor (nfo_tvshow_episode_t *ep, int id)
{
  int max;

  if (!ep)
    return NULL;

  max = list_get_length (ep->actors);
  if (id >= max)
    return NULL;

  return ep->actors[id];
}

int
nfo_tvshow_episode_get_video_streams_count (nfo_tvshow_episode_t *ep)
{
  nfo_fileinfo_t *fileinfo;

  if (!ep || !ep->fileinfo)
    return 0;

  fileinfo = ep->fileinfo;
  return fileinfo->videos ? list_get_length (fileinfo->videos) : 0;
}

nfo_stream_video_t *
nfo_tvshow_episode_get_video_stream (nfo_tvshow_episode_t *ep, int id)
{
  nfo_fileinfo_t *fileinfo;
  int max;

  if (!ep || !ep->fileinfo)
    return NULL;

  fileinfo = ep->fileinfo;
  max = list_get_length (fileinfo->videos);
  if (id >= max)
    return NULL;

  return fileinfo->videos[id];
}

int
nfo_tvshow_episode_get_audio_streams_count (nfo_tvshow_episode_t *ep)
{
  nfo_fileinfo_t *fileinfo;

  if (!ep || !ep->fileinfo)
    return 0;

  fileinfo = ep->fileinfo;
  return fileinfo->audios ? list_get_length (fileinfo->audios) : 0;
}

nfo_stream_audio_t *
nfo_tvshow_episode_get_audio_stream (nfo_tvshow_episode_t *ep, int id)
{
  nfo_fileinfo_t *fileinfo;
  int max;

  if (!ep || !ep->fileinfo)
    return NULL;

  fileinfo = ep->fileinfo;
  max = list_get_length (fileinfo->audios);
  if (id >= max)
    return NULL;

  return fileinfo->audios[id];
}

int
nfo_tvshow_episode_get_sub_streams_count (nfo_tvshow_episode_t *ep)
{
  nfo_fileinfo_t *fileinfo;

  if (!ep || !ep->fileinfo)
    return 0;

  fileinfo = ep->fileinfo;
  return fileinfo->subs ? list_get_length (fileinfo->subs) : 0;
}

nfo_stream_sub_t *
nfo_tvshow_episode_get_sub_stream (nfo_tvshow_episode_t *ep, int id)
{
  nfo_fileinfo_t *fileinfo;
  int max;

  if (!ep || !ep->fileinfo)
    return NULL;

  fileinfo = ep->fileinfo;
  max = list_get_length (fileinfo->subs);
  if (id >= max)
    return NULL;

  return fileinfo->subs[id];
}

nfo_tvshow_t *
nfo_tvshow_episode_get_show (nfo_tvshow_episode_t *ep)
{
  return ep ? ep->show : NULL;
}

char *
nfo_tvshow_get (nfo_tvshow_t *ep, nfo_tvshow_field_t f)
{
  if (!ep)
    return NULL;

  switch (f)
  {
  case NFO_TVSHOW_TITLE:
    return ep->title;
  case NFO_TVSHOW_RATING:
    return ep->rating;
  case NFO_TVSHOW_SEASON:
    return ep->season;
  case NFO_TVSHOW_EPISODE:
    return ep->episode;
  case NFO_TVSHOW_DISPLAY_SEASON:
    return ep->displayseason;
  case NFO_TVSHOW_DISPLAY_EPISODE:
    return ep->displayepisode;
  case NFO_TVSHOW_EPISODE_GUIDE_URL:
    return ep->episode_guide_url;
  case NFO_TVSHOW_PLOT:
    return ep->plot;
  case NFO_TVSHOW_MPAA:
    return ep->mpaa;
  case NFO_TVSHOW_WATCHED:
    return ep->watched;
  case NFO_TVSHOW_GENRE:
    return ep->genre;
  case NFO_TVSHOW_PREMIERED:
    return ep->premiered;
  case NFO_TVSHOW_STUDIO:
    return ep->studio;
    }

  return NULL;
}

char *
nfo_actor_get (nfo_actor_t *actor, nfo_actor_field_t f)
{
  if (!actor)
    return NULL;

  switch (f)
  {
  case NFO_ACTOR_NAME:
    return actor->name;
  case NFO_ACTOR_ROLE:
    return actor->role;
  case NFO_ACTOR_THUMB:
    return actor->thumb;
  }

  return NULL;
}

char *
nfo_video_stream_get (nfo_stream_video_t *video, nfo_video_field_t f)
{
  if (!video)
    return NULL;

  switch (f)
  {
  case NFO_VIDEO_WIDTH:
    return video->width;
  case NFO_VIDEO_HEIGHT:
    return video->height;
  case NFO_VIDEO_CODEC:
    return video->codec;
  case NFO_VIDEO_FORMAT_INFO:
    return video->format_info;
  case NFO_VIDEO_DURATION:
    return video->duration;
  case NFO_VIDEO_BITRATE:
    return video->bitrate;
  case NFO_VIDEO_BITRATE_MODE:
    return video->bitrate_mode;
  case NFO_VIDEO_BITRATE_MAX:
    return video->bitrate_max;
  case NFO_VIDEO_CONTAINER:
    return video->container;
  case NFO_VIDEO_CODEC_ID:
    return video->codec_id;
  case NFO_VIDEO_CODEC_INFO:
    return video->codec_info;
  case NFO_VIDEO_SCAN_TYPE:
    return video->scan_type;
  case NFO_VIDEO_ASPECT:
    return video->aspect;
  }

  return NULL;
}

char *
nfo_audio_stream_get (nfo_stream_audio_t *audio, nfo_audio_field_t f)
{
  if (!audio)
    return NULL;

  switch (f)
  {
  case NFO_AUDIO_LANG:
    return audio->lang;
  case NFO_AUDIO_CODEC:
    return audio->codec;
  case NFO_AUDIO_CHANNELS:
    return audio->channels;
  case NFO_AUDIO_BITRATE:
    return audio->bitrate;
  }

  return NULL;
}

char *
nfo_sub_stream_get (nfo_stream_sub_t *sub, nfo_sub_field_t f)
{
  if (!sub)
    return NULL;

  switch (f)
  {
  case NFO_SUB_LANG:
    return sub->lang;
  }

  return NULL;
}
