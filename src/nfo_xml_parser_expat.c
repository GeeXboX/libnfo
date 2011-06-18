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

#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <stdarg.h>

#include <expat.h>

#include "nfo.h"
#include "nfo_osdep.h"
#include "nfo_priv.h"

typedef enum {
  XML_TAG_INVALID,
  XML_TAG_MOVIE,
  XML_TAG_EPISODE_DETAILS,
  XML_TAG_TITLE,
  XML_TAG_ORIGINAL_TITLE,
  XML_TAG_RATING,
  XML_TAG_YEAR,
  XML_TAG_TOP250,
  XML_TAG_VOTES,
  XML_TAG_OUTLINE,
  XML_TAG_PLOT,
  XML_TAG_TAGLINE,
  XML_TAG_RUNTIME,
  XML_TAG_THUMB,
  XML_TAG_MPAA,
  XML_TAG_PLAYCOUNT,
  XML_TAG_ID,
  XML_TAG_GENRE,
  XML_TAG_CREDITS,
  XML_TAG_DIRECTOR,
  XML_TAG_PREMIERED,
  XML_TAG_STUDIO,
  XML_TAG_TRAILER,
  XML_TAG_WATCHED,
  XML_TAG_SEASON,
  XML_TAG_EPISODE,
  XML_TAG_AIRED,

  XML_TAG_ACTOR,
  XML_TAG_ACTOR_NAME,
  XML_TAG_ACTOR_ROLE,
  XML_TAG_ACTOR_THUMB,

  XML_TAG_FILEINFO,
  XML_TAG_STREAMDETAILS,
  XML_TAG_SUBTITLE,
  XML_TAG_SUBTITLE_LANG,
  XML_TAG_AUDIO,
  XML_TAG_AUDIO_LANG,
  XML_TAG_AUDIO_CODEC,
  XML_TAG_AUDIO_CHANNELS,
  XML_TAG_AUDIO_BITRATE,
  XML_TAG_VIDEO,
  XML_TAG_VIDEO_WIDTH,
  XML_TAG_VIDEO_HEIGHT,
  XML_TAG_VIDEO_CODEC,
  XML_TAG_VIDEO_FORMATINFO,
  XML_TAG_VIDEO_DURATION,
  XML_TAG_VIDEO_BITRATE,
  XML_TAG_VIDEO_BITRATEMODE,
  XML_TAG_VIDEO_BITRATEMAX,
  XML_TAG_VIDEO_CONTAINER,
  XML_TAG_VIDEO_CODEC_ID,
  XML_TAG_VIDEO_CODEC_ID_INFO,
  XML_TAG_VIDEO_SCANTYPE,
  XML_TAG_VIDEO_ASPECT_RATIO,
} xml_tag_name;

typedef struct movie_data_s {
  XML_Parser p;
  nfo_type_t type;
  xml_tag_name current_tag;
  nfo_actor_t *current_actor;
  nfo_stream_sub_t *current_str_sub;
  nfo_stream_audio_t *current_str_audio;
  nfo_stream_video_t *current_str_video;
  nfo_movie_t *movie;
  nfo_tvshow_episode_t *episode;
} movie_data_t;

/* string utilities */
static char *
strdup_vprintf (const char *fmt, va_list ap)
{
  size_t len = 80;
  char *buf;

  buf = malloc (len);

  for (;;)
  {
    int n;
    char *nbuf;
    va_list ap2;

    va_copy (ap2, ap);
    n = vsnprintf (buf, len, fmt, ap2);
    va_end (ap2);

    if (n >= 0 && n < (int) len)
      return buf;

    if (n >= 0)
      len = n + 1;
    else
      len *= 2;

    nbuf = realloc (buf, len);
    if (!nbuf)
    {
      NFREE (buf);
      return NULL;
    }

    buf = nbuf;
  }
}

static char *
strdup_printf (const char *fmt, ... )
{
  char *s;
  va_list ap;

  va_start (ap, fmt);
  s = strdup_vprintf (fmt, ap);
  va_end (ap);

  return s;
}

/* file parser */
static char *
nfo_file_exists (const char *dir, const char *file, const char *ext)
{
  struct stat st;
  char nfo_file[1024] = { 0 };
  int err;

  snprintf (nfo_file, sizeof (nfo_file), "%s/%s.%s", dir, file, ext);
  err = stat (nfo_file, &st);

  return !err ? strdup (nfo_file) : NULL;
}

static void
nfo_grab_tvshow_tbn (nfo_tvshow_episode_t *episode,
                     const char *dir, const char *file)
{
  char *s;

  if (!episode || !dir || !file)
    return;

  episode->fanart = nfo_file_exists (dir, file, "tbn");

  s = strrchr (dir, '/');
  if (s)
  {
    char tbn_season[1024] = { 0 };
    char *parent_path;
    int err, season;
    struct stat st;

    parent_path = strndup (dir, strlen (dir) - strlen (s));

    if (episode->season)
      season = atoi (episode->season);
    else /* try to guess from dirname, must be ../tvshow/S[0-9]/episode.. */
    {
      char guess[16] = { 0 };
      snprintf (guess, sizeof (guess), "%s", s + 2);
      season = atoi (guess);
    }

    snprintf (tbn_season, sizeof (tbn_season),
              "%s/season%.2d.tbn", parent_path, season);

    err = stat (tbn_season, &st);
    episode->fanart_season = !err ? strdup (tbn_season) : NULL;

    if (episode->show)
    {
      char tbn[1024] = { 0 };
      nfo_tvshow_t *show = episode->show;

      memset (tbn, '\0', sizeof (tbn));
      snprintf (tbn, sizeof (tbn), "%s/fanart.jpg", parent_path);
      err = stat (tbn, &st);
      show->fanart = !err ? strdup (tbn) : NULL;

      memset (tbn, '\0', sizeof (tbn));
      snprintf (tbn, sizeof (tbn), "%s/folder.jpg", parent_path);
      err = stat (tbn, &st);
      show->fanart_header = !err ? strdup (tbn) : NULL;

      memset (tbn, '\0', sizeof (tbn));
      snprintf (tbn, sizeof (tbn), "%s/season-all.tbn", parent_path);
      err = stat (tbn, &st);
      show->fanart_cover = !err ? strdup (tbn) : NULL;
    }

    NFREE (parent_path);
  }
}

static void
nfo_xml_parser_start (void *data, const char *el, const char **attr)
{
  movie_data_t *d = data;
  nfo_actor_t *actor;
  attr = NULL;

  switch (d->current_tag)
  {
  case XML_TAG_INVALID:
    if (!strcasecmp (el, "movie"))
    {
      d->type = NFO_MOVIE;
      d->movie = nfo_movie_new ();
      d->current_tag = XML_TAG_MOVIE;
    }
    else if (!strcasecmp (el, "episodedetails"))
    {
      d->type = NFO_TVSHOW;
      d->episode = nfo_tvshow_episode_new ();
      d->current_tag = XML_TAG_EPISODE_DETAILS;
    }
    else
      return;
    break;

  case XML_TAG_MOVIE:
    if (!strcasecmp (el, "title"))
      d->current_tag = XML_TAG_TITLE;
    else if (!strcasecmp (el, "originaltitle"))
      d->current_tag = XML_TAG_ORIGINAL_TITLE;
    else if (!strcasecmp (el, "rating"))
      d->current_tag = XML_TAG_RATING;
    else if (!strcasecmp (el, "year"))
      d->current_tag = XML_TAG_YEAR;
    else if (!strcasecmp (el, "top250"))
      d->current_tag = XML_TAG_TOP250;
    else if (!strcasecmp (el, "votes"))
      d->current_tag = XML_TAG_VOTES;
    else if (!strcasecmp (el, "outline"))
      d->current_tag = XML_TAG_OUTLINE;
    else if (!strcasecmp (el, "plot"))
      d->current_tag = XML_TAG_PLOT;
    else if (!strcasecmp (el, "tagline"))
      d->current_tag = XML_TAG_TAGLINE;
    else if (!strcasecmp (el, "runtime"))
      d->current_tag = XML_TAG_RUNTIME;
    else if (!strcasecmp (el, "thumb"))
      d->current_tag = XML_TAG_THUMB;
    else if (!strcasecmp (el, "mpaa"))
      d->current_tag = XML_TAG_MPAA;
    else if (!strcasecmp (el, "playcount"))
      d->current_tag = XML_TAG_PLAYCOUNT;
    else if (!strcasecmp (el, "id"))
      d->current_tag = XML_TAG_ID;
    else if (!strcasecmp (el, "genre"))
      d->current_tag = XML_TAG_GENRE;
    else if (!strcasecmp (el, "credits"))
      d->current_tag = XML_TAG_CREDITS;
    else if (!strcasecmp (el, "director"))
      d->current_tag = XML_TAG_DIRECTOR;
    else if (!strcasecmp (el, "premiered"))
      d->current_tag = XML_TAG_PREMIERED;
    else if (!strcasecmp (el, "studio"))
      d->current_tag = XML_TAG_STUDIO;
    else if (!strcasecmp (el, "trailer"))
      d->current_tag = XML_TAG_TRAILER;
    else if (!strcasecmp (el, "watched"))
      d->current_tag = XML_TAG_WATCHED;
    else if (!strcasecmp (el, "actor"))
    {
      d->current_tag = XML_TAG_ACTOR;
      actor = nfo_actor_new ();
      d->current_actor = actor;
      nfo_movie_add_actor (d->movie, actor);
    }
    else if (!strcasecmp (el, "fileinfo"))
      d->current_tag = XML_TAG_FILEINFO;
    break;

  case XML_TAG_EPISODE_DETAILS:
    if (!strcasecmp (el, "title"))
      d->current_tag = XML_TAG_TITLE;
    else if (!strcasecmp (el, "Rating"))
      d->current_tag = XML_TAG_RATING;
    else if (!strcasecmp (el, "season"))
      d->current_tag = XML_TAG_SEASON;
    else if (!strcasecmp (el, "episode"))
      d->current_tag = XML_TAG_EPISODE;
    else if (!strcasecmp (el, "plot"))
      d->current_tag = XML_TAG_PLOT;
    else if (!strcasecmp (el, "thumb"))
      d->current_tag = XML_TAG_THUMB;
    else if (!strcasecmp (el, "playcount"))
      d->current_tag = XML_TAG_PLAYCOUNT;
    else if (!strcasecmp (el, "credits"))
      d->current_tag = XML_TAG_CREDITS;
    else if (!strcasecmp (el, "director"))
      d->current_tag = XML_TAG_DIRECTOR;
    else if (!strcasecmp (el, "aired"))
      d->current_tag = XML_TAG_AIRED;
    else if (!strcasecmp (el, "votes"))
      d->current_tag = XML_TAG_VOTES;
    else if (!strcasecmp (el, "actor"))
    {
      d->current_tag = XML_TAG_ACTOR;
      actor = nfo_actor_new ();
      d->current_actor = actor;
      nfo_tvshow_episode_add_actor (d->episode, actor);
    }
    else if (!strcasecmp (el, "fileinfo"))
      d->current_tag = XML_TAG_FILEINFO;
    break;

  case XML_TAG_TITLE:
  case XML_TAG_ORIGINAL_TITLE:
  case XML_TAG_RATING:
  case XML_TAG_YEAR:
  case XML_TAG_TOP250:
  case XML_TAG_VOTES:
  case XML_TAG_OUTLINE:
  case XML_TAG_PLOT:
  case XML_TAG_TAGLINE:
  case XML_TAG_RUNTIME:
  case XML_TAG_THUMB:
  case XML_TAG_MPAA:
  case XML_TAG_PLAYCOUNT:
  case XML_TAG_ID:
  case XML_TAG_GENRE:
  case XML_TAG_CREDITS:
  case XML_TAG_DIRECTOR:
  case XML_TAG_PREMIERED:
  case XML_TAG_STUDIO:
  case XML_TAG_TRAILER:
  case XML_TAG_WATCHED:
  case XML_TAG_SEASON:
  case XML_TAG_EPISODE:
  case XML_TAG_AIRED:
    break;

  case XML_TAG_ACTOR:
    if (!strcasecmp (el, "name"))
      d->current_tag = XML_TAG_ACTOR_NAME;
    else if (!strcasecmp (el, "role"))
      d->current_tag = XML_TAG_ACTOR_ROLE;
    else if (!strcasecmp (el, "thumb"))
      d->current_tag = XML_TAG_ACTOR_THUMB;
    break;

  case XML_TAG_ACTOR_NAME:
  case XML_TAG_ACTOR_ROLE:
  case XML_TAG_ACTOR_THUMB:
    break;

  case XML_TAG_FILEINFO:
    if (!strcasecmp (el, "streamdetails"))
    {
      nfo_fileinfo_t *fileinfo;
      d->current_tag = XML_TAG_STREAMDETAILS;
      fileinfo = nfo_fileinfo_new ();
      if (d->type == NFO_MOVIE)
        d->movie->fileinfo = fileinfo;
      else if (d->type == NFO_TVSHOW)
        d->episode->fileinfo = fileinfo;
    }
    break;

  case XML_TAG_STREAMDETAILS:
    if (!strcasecmp (el, "subtitle"))
    {
      nfo_stream_sub_t *sub;
      sub = nfo_stream_sub_new ();
      if (d->type == NFO_MOVIE)
        nfo_fileinfo_add_stream_sub (d->movie->fileinfo, sub);
      else if (d->type == NFO_TVSHOW)
        nfo_fileinfo_add_stream_sub (d->episode->fileinfo, sub);
      d->current_tag = XML_TAG_SUBTITLE;
      d->current_str_sub = sub;
    }
    else if (!strcasecmp (el, "audio"))
    {
      nfo_stream_audio_t *audio;
      audio = nfo_stream_audio_new ();
      if (d->type == NFO_MOVIE)
        nfo_fileinfo_add_stream_audio (d->movie->fileinfo, audio);
      else if (d->type == NFO_TVSHOW)
        nfo_fileinfo_add_stream_audio (d->episode->fileinfo, audio);
      d->current_tag = XML_TAG_AUDIO;
      d->current_str_audio = audio;
    }
    else if (!strcasecmp (el, "video"))
    {
      nfo_stream_video_t *video;
      video = nfo_stream_video_new ();
      if (d->type == NFO_MOVIE)
        nfo_fileinfo_add_stream_video (d->movie->fileinfo, video);
      else if (d->type == NFO_TVSHOW)
        nfo_fileinfo_add_stream_video (d->episode->fileinfo, video);
      d->current_tag = XML_TAG_VIDEO;
      d->current_str_video = video;
    }
    break;

  case XML_TAG_SUBTITLE:
    if (!strcasecmp (el, "language"))
      d->current_tag = XML_TAG_SUBTITLE_LANG;
    break;

  case XML_TAG_SUBTITLE_LANG:
    break;

  case XML_TAG_AUDIO:
    if (!strcasecmp (el, "language"))
      d->current_tag = XML_TAG_AUDIO_LANG;
    if (!strcasecmp (el, "codec"))
      d->current_tag = XML_TAG_AUDIO_CODEC;
    if (!strcasecmp (el, "channels"))
      d->current_tag = XML_TAG_AUDIO_CHANNELS;
    if (!strcasecmp (el, "bitrate"))
      d->current_tag = XML_TAG_AUDIO_BITRATE;
    break;

  case XML_TAG_AUDIO_LANG:
  case XML_TAG_AUDIO_CODEC:
  case XML_TAG_AUDIO_CHANNELS:
  case XML_TAG_AUDIO_BITRATE:
    break;

  case XML_TAG_VIDEO:
    if (!strcasecmp (el, "width"))
      d->current_tag = XML_TAG_VIDEO_WIDTH;
    else if (!strcasecmp (el, "height"))
      d->current_tag = XML_TAG_VIDEO_HEIGHT;
    else if (!strcasecmp (el, "codec"))
      d->current_tag = XML_TAG_VIDEO_CODEC;
    else if (!strcasecmp (el, "formatinfo"))
      d->current_tag = XML_TAG_VIDEO_FORMATINFO;
    else if (!strcasecmp (el, "duration"))
      d->current_tag = XML_TAG_VIDEO_DURATION;
    else if (!strcasecmp (el, "bitrate"))
      d->current_tag = XML_TAG_VIDEO_BITRATE;
    else if (!strcasecmp (el, "bitratemode"))
      d->current_tag = XML_TAG_VIDEO_BITRATEMODE;
    else if (!strcasecmp (el, "bitratemax"))
      d->current_tag = XML_TAG_VIDEO_BITRATEMAX;
    else if (!strcasecmp (el, "container"))
      d->current_tag = XML_TAG_VIDEO_CONTAINER;
    else if (!strcasecmp (el, "codecid"))
      d->current_tag = XML_TAG_VIDEO_CODEC_ID;
    else if (!strcasecmp (el, "codecidinfo"))
      d->current_tag = XML_TAG_VIDEO_CODEC_ID_INFO;
    else if (!strcasecmp (el, "scantype"))
      d->current_tag = XML_TAG_VIDEO_SCANTYPE;
    else if (!strcasecmp (el, "aspectdisplayratio"))
      d->current_tag = XML_TAG_VIDEO_ASPECT_RATIO;
    break;

  case XML_TAG_VIDEO_WIDTH:
  case XML_TAG_VIDEO_HEIGHT:
  case XML_TAG_VIDEO_CODEC:
  case XML_TAG_VIDEO_FORMATINFO:
  case XML_TAG_VIDEO_DURATION:
  case XML_TAG_VIDEO_BITRATE:
  case XML_TAG_VIDEO_BITRATEMODE:
  case XML_TAG_VIDEO_BITRATEMAX:
  case XML_TAG_VIDEO_CONTAINER:
  case XML_TAG_VIDEO_CODEC_ID:
  case XML_TAG_VIDEO_CODEC_ID_INFO:
  case XML_TAG_VIDEO_SCANTYPE:
  case XML_TAG_VIDEO_ASPECT_RATIO:
    break;
  }
}

static void
nfo_xml_parser_end (void *data, const char *el)
{
  movie_data_t *d = data;
  el = NULL;

  switch (d->current_tag)
  {

  case XML_TAG_INVALID:
    break;

  case XML_TAG_MOVIE:
  case XML_TAG_EPISODE_DETAILS:
    d->current_tag = XML_TAG_INVALID;
    break;

  case XML_TAG_TITLE:
  case XML_TAG_ORIGINAL_TITLE:
  case XML_TAG_RATING:
  case XML_TAG_YEAR:
  case XML_TAG_TOP250:
  case XML_TAG_VOTES:
  case XML_TAG_OUTLINE:
  case XML_TAG_PLOT:
  case XML_TAG_TAGLINE:
  case XML_TAG_RUNTIME:
  case XML_TAG_THUMB:
  case XML_TAG_MPAA:
  case XML_TAG_PLAYCOUNT:
  case XML_TAG_ID:
  case XML_TAG_GENRE:
  case XML_TAG_CREDITS:
  case XML_TAG_DIRECTOR:
  case XML_TAG_PREMIERED:
  case XML_TAG_STUDIO:
  case XML_TAG_TRAILER:
  case XML_TAG_WATCHED:
  case XML_TAG_SEASON:
  case XML_TAG_EPISODE:
  case XML_TAG_AIRED:
  case XML_TAG_ACTOR:
  case XML_TAG_FILEINFO:
    if (d->type == NFO_MOVIE)
      d->current_tag = XML_TAG_MOVIE;
    else if (d->type == NFO_TVSHOW)
      d->current_tag = XML_TAG_EPISODE_DETAILS;
    break;

  case XML_TAG_ACTOR_NAME:
  case XML_TAG_ACTOR_ROLE:
  case XML_TAG_ACTOR_THUMB:
    d->current_tag = XML_TAG_ACTOR;
    break;

  case XML_TAG_STREAMDETAILS:
    d->current_tag = XML_TAG_FILEINFO;
    break;

  case XML_TAG_SUBTITLE:
  case XML_TAG_AUDIO:
  case XML_TAG_VIDEO:
    d->current_tag = XML_TAG_STREAMDETAILS;
    break;

  case XML_TAG_SUBTITLE_LANG:
    d->current_tag = XML_TAG_SUBTITLE;
    break;

  case XML_TAG_AUDIO_LANG:
  case XML_TAG_AUDIO_CODEC:
  case XML_TAG_AUDIO_CHANNELS:
  case XML_TAG_AUDIO_BITRATE:
    d->current_tag = XML_TAG_AUDIO;
    break;

  case XML_TAG_VIDEO_WIDTH:
  case XML_TAG_VIDEO_HEIGHT:
  case XML_TAG_VIDEO_CODEC:
  case XML_TAG_VIDEO_FORMATINFO:
  case XML_TAG_VIDEO_DURATION:
  case XML_TAG_VIDEO_BITRATE:
  case XML_TAG_VIDEO_BITRATEMODE:
  case XML_TAG_VIDEO_BITRATEMAX:
  case XML_TAG_VIDEO_CONTAINER:
  case XML_TAG_VIDEO_CODEC_ID:
  case XML_TAG_VIDEO_CODEC_ID_INFO:
  case XML_TAG_VIDEO_SCANTYPE:
  case XML_TAG_VIDEO_ASPECT_RATIO:
    d->current_tag = XML_TAG_VIDEO;
    break;
  }
}

static char *
append_cdata (char *t, const char *n, int length)
{
  char *r, *k;

  if (!length)
    return t;

  k = strndup (n, length);

  if (t)
  {
    r = strdup_printf ("%s%s", t, k);
    NFREE (k);
    NFREE (t);
  }
  else
    r = k;

  return r;
}

static void
nfo_xml_fill_cdata (movie_data_t *d, xml_tag_name tag, char **c,
                    const XML_Char *s, int len)
{
  if (d->current_tag != tag)
    return;

  *c = append_cdata (*c, s, len);
}

#define FILL_CDATA(tag, field) \
  nfo_xml_fill_cdata (d, tag, field, s, len);

static void
nfo_xml_parser_cdata (void *data, const XML_Char *s, int len)
{
  movie_data_t *d = data;

  if (d->type == NFO_MOVIE)
  {
    FILL_CDATA (XML_TAG_TITLE,             &d->movie->title);
    FILL_CDATA (XML_TAG_ORIGINAL_TITLE,    &d->movie->original_title);
    FILL_CDATA (XML_TAG_RATING,            &d->movie->rating);
    FILL_CDATA (XML_TAG_YEAR,              &d->movie->year);
    FILL_CDATA (XML_TAG_TOP250,            &d->movie->top250);
    FILL_CDATA (XML_TAG_VOTES,             &d->movie->votes);
    FILL_CDATA (XML_TAG_OUTLINE,           &d->movie->outline);
    FILL_CDATA (XML_TAG_PLOT,              &d->movie->plot);
    FILL_CDATA (XML_TAG_TAGLINE,           &d->movie->tagline);
    FILL_CDATA (XML_TAG_RUNTIME,           &d->movie->runtime);
    FILL_CDATA (XML_TAG_THUMB,             &d->movie->thumb);
    FILL_CDATA (XML_TAG_MPAA,              &d->movie->mpaa);
    FILL_CDATA (XML_TAG_PLAYCOUNT,         &d->movie->playcount);
    FILL_CDATA (XML_TAG_WATCHED,           &d->movie->watched);
    FILL_CDATA (XML_TAG_ID,                &d->movie->id);
    FILL_CDATA (XML_TAG_TRAILER,           &d->movie->trailer);
    FILL_CDATA (XML_TAG_GENRE,             &d->movie->genre);
    FILL_CDATA (XML_TAG_CREDITS,           &d->movie->credits);
    FILL_CDATA (XML_TAG_DIRECTOR,          &d->movie->director);
    FILL_CDATA (XML_TAG_STUDIO,            &d->movie->studio);
  }
  else if (d->type == NFO_TVSHOW)
  {
    FILL_CDATA (XML_TAG_TITLE,             &d->episode->title);
    FILL_CDATA (XML_TAG_RATING,            &d->episode->rating);
    FILL_CDATA (XML_TAG_SEASON,            &d->episode->season);
    FILL_CDATA (XML_TAG_EPISODE,           &d->episode->episode);
    FILL_CDATA (XML_TAG_PLOT,              &d->episode->plot);
    FILL_CDATA (XML_TAG_THUMB,             &d->episode->thumb);
    FILL_CDATA (XML_TAG_PLAYCOUNT,         &d->episode->playcount);
    FILL_CDATA (XML_TAG_CREDITS,           &d->episode->credits);
    FILL_CDATA (XML_TAG_DIRECTOR,          &d->episode->director);
    FILL_CDATA (XML_TAG_AIRED,             &d->episode->aired);
    FILL_CDATA (XML_TAG_VOTES,             &d->episode->votes);
  }

  FILL_CDATA (XML_TAG_ACTOR_NAME,          &d->current_actor->name);
  FILL_CDATA (XML_TAG_ACTOR_ROLE,          &d->current_actor->role);
  FILL_CDATA (XML_TAG_ACTOR_THUMB,         &d->current_actor->thumb);

  FILL_CDATA (XML_TAG_SUBTITLE_LANG,       &d->current_str_sub->lang);

  FILL_CDATA (XML_TAG_AUDIO_LANG,          &d->current_str_audio->lang);
  FILL_CDATA (XML_TAG_AUDIO_CODEC,         &d->current_str_audio->codec);
  FILL_CDATA (XML_TAG_AUDIO_CHANNELS,      &d->current_str_audio->channels);
  FILL_CDATA (XML_TAG_AUDIO_BITRATE,       &d->current_str_audio->bitrate);

  FILL_CDATA (XML_TAG_VIDEO_WIDTH,         &d->current_str_video->width);
  FILL_CDATA (XML_TAG_VIDEO_HEIGHT,        &d->current_str_video->height);
  FILL_CDATA (XML_TAG_VIDEO_CODEC,         &d->current_str_video->codec);
  FILL_CDATA (XML_TAG_VIDEO_FORMATINFO,    &d->current_str_video->format_info);
  FILL_CDATA (XML_TAG_VIDEO_DURATION,      &d->current_str_video->duration);
  FILL_CDATA (XML_TAG_VIDEO_BITRATE,       &d->current_str_video->bitrate);
  FILL_CDATA (XML_TAG_VIDEO_BITRATEMODE,   &d->current_str_video->bitrate_mode);
  FILL_CDATA (XML_TAG_VIDEO_BITRATEMAX,    &d->current_str_video->bitrate_max);
  FILL_CDATA (XML_TAG_VIDEO_CONTAINER,     &d->current_str_video->container);
  FILL_CDATA (XML_TAG_VIDEO_CODEC_ID,      &d->current_str_video->codec_id);
  FILL_CDATA (XML_TAG_VIDEO_CODEC_ID_INFO, &d->current_str_video->codec_info);
  FILL_CDATA (XML_TAG_VIDEO_SCANTYPE,      &d->current_str_video->scan_type);
  FILL_CDATA (XML_TAG_VIDEO_ASPECT_RATIO,  &d->current_str_video->aspect);
}

static void
nfo_parse_xml_file (nfo_t *nfo, const char *filename,
                    const char *dir, const char *file)
{
  XML_Parser p;
  movie_data_t data;

  struct stat st;
  char *buf;
  int fd, n;

  stat (filename, &st);
  buf = malloc (st.st_size + 1);
  fd = open (filename, O_RDONLY);
  n = read (fd, buf, st.st_size);
  close (fd);

  p = XML_ParserCreate (NULL);
  if (!p)
    return;

  data.p                 = p;
  data.type              = NFO_UNKNOWN;
  data.current_tag       = XML_TAG_INVALID;
  data.current_actor     = NULL;
  data.current_str_sub   = NULL;
  data.current_str_audio = NULL;
  data.current_str_video = NULL;
  data.movie             = NULL;
  data.episode           = NULL;

  XML_SetUserData (p, &data);
  XML_SetElementHandler (p, nfo_xml_parser_start, nfo_xml_parser_end);
  XML_SetCharacterDataHandler (p, nfo_xml_parser_cdata);
  XML_Parse (p, buf, st.st_size, 1);

  if (data.type == NFO_MOVIE)
  {
    nfo->type = NFO_MOVIE;
    nfo->movie = data.movie;
    nfo->movie->fanart = nfo_file_exists (dir, file, "tbn");
  }
  else if (data.type == NFO_TVSHOW)
  {
    nfo->type = NFO_TVSHOW;
    nfo->tvshow = data.episode;
    nfo_grab_tvshow_tbn (nfo->tvshow, dir, file);
  }

  NFREE (buf);
  XML_ParserFree (p);
}

void
nfo_parse_xml (nfo_t *nfo, const char *filename)
{
  char *s, *dir = NULL, *file = NULL, *nfo_file = NULL;

  if (!nfo || !filename)
    return;

  /* retrieve directory name */
  s = strrchr (filename, '/');
  if (!s)
    goto nfo_parse_xml_end;

  dir = strndup (filename, strlen (filename) - strlen (s));

  /* retrieve file base name */
  s = strrchr (filename, '.');
  if (!s)
    goto nfo_parse_xml_end;
  file = strndup (filename + strlen (dir) + 1,
                  strlen (filename) - strlen (dir) - strlen (s) - 1);

  nfo_file = nfo_file_exists (dir, file, "nfo");

  if (nfo_file)
  {
    nfo_parse_xml_file (nfo, nfo_file, dir, file);
    if (nfo->type == NFO_MOVIE || nfo->type == NFO_TVSHOW)
      goto nfo_parse_xml_end;
  }

  nfo_file = nfo_file_exists (dir, "movie", "nfo");
  if (nfo_file)
  {
    nfo_parse_xml_file (nfo, nfo_file, dir, file);
    goto nfo_parse_xml_end;
  }

  nfo_file = nfo_file_exists (dir, "Movie", "nfo");
  if (nfo_file)
  {
    nfo_parse_xml_file (nfo, nfo_file, dir, file);
    goto nfo_parse_xml_end;
  }

 nfo_parse_xml_end:
  NFREE (dir);
  NFREE (file);
  NFREE (nfo_file);
}
