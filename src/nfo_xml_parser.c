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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

#include "nfo.h"
#include "nfo_priv.h"
#include "nfo_xml_utils.h"

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

static int
nfo_xml_search_str (xmlNode *n, const char *node, char **str)
{
  xmlChar *tmp;

  if (*str)
    return 1;

  tmp = nfo_get_prop_value_from_xml_tree (n, node);
  if (!tmp)
    return 1;

  *str = strdup ((char *) tmp);
  xmlFree (tmp);

  return 0;
}

static void
nfo_parse_xml_video (nfo_fileinfo_t *info, xmlNode *node)
{
  xmlNode *n;

  if (!info || !node)
    return;

  n = nfo_get_node_xml_tree (node, "video");
  while (n)
  {
    nfo_stream_video_t *video;

    if (strcmp ((const char *) n->name, "video"))
    {
      n = n->next;
      continue;
    }

    video = nfo_stream_video_new ();
    nfo_xml_search_str (n, "width",              &video->width);
    nfo_xml_search_str (n, "height",             &video->height);
    nfo_xml_search_str (n, "codec",              &video->codec);
    nfo_xml_search_str (n, "formatinfo",         &video->format_info);
    nfo_xml_search_str (n, "duration",           &video->duration);
    nfo_xml_search_str (n, "bitrate",            &video->bitrate);
    nfo_xml_search_str (n, "bitratemode",        &video->bitrate_mode);
    nfo_xml_search_str (n, "bitratemax",         &video->bitrate_max);
    nfo_xml_search_str (n, "container",          &video->container);
    nfo_xml_search_str (n, "codecid",            &video->codec_id);
    nfo_xml_search_str (n, "codecidinfo",        &video->codec_info);
    nfo_xml_search_str (n, "scantype",           &video->scan_type);
    nfo_xml_search_str (n, "aspectdisplayratio", &video->aspect);

    nfo_fileinfo_add_stream_video (info, video);
    n = n->next;
  }
}

static void
nfo_parse_xml_audio (nfo_fileinfo_t *info, xmlNode *node)
{
  xmlNode *n;

  if (!info || !node)
    return;

  n = nfo_get_node_xml_tree (node, "audio");
  while (n)
  {
    nfo_stream_audio_t *audio;

    if (strcmp ((const char *) n->name, "audio"))
    {
      n = n->next;
      continue;
    }

    audio = nfo_stream_audio_new ();
    nfo_xml_search_str (n, "language", &audio->lang);
    nfo_xml_search_str (n, "codec",    &audio->codec);
    nfo_xml_search_str (n, "channels", &audio->channels);
    nfo_xml_search_str (n, "bitrate",  &audio->bitrate);

    nfo_fileinfo_add_stream_audio (info, audio);
    n = n->next;
  }
}

static void
nfo_parse_xml_sub (nfo_fileinfo_t *info, xmlNode *node)
{
  xmlNode *n;

  if (!info || !node)
    return;

  n = nfo_get_node_xml_tree (node, "subtitle");
  while (n)
  {
    nfo_stream_sub_t *sub;

    if (strcmp ((const char *) n->name, "subtitle"))
    {
      n = n->next;
      continue;
    }

    sub = nfo_stream_sub_new ();
    nfo_xml_search_str (n, "language", &sub->lang);

    nfo_fileinfo_add_stream_sub (info, sub);
    n = n->next;
  }
}

static void
nfo_movie_parse_xml_fileinfo (nfo_movie_t *movie, xmlNode *n)
{
  nfo_fileinfo_t *fileinfo;
  xmlNode *sd;

  if (!movie || !n)
    return;

  sd = nfo_get_node_xml_tree (n, "streamdetails");
  if (!sd)
    return;

  fileinfo = nfo_fileinfo_new ();
  nfo_parse_xml_video (fileinfo, sd);
  nfo_parse_xml_audio (fileinfo, sd);
  nfo_parse_xml_sub   (fileinfo, sd);

  movie->fileinfo = fileinfo;
}

static void
nfo_movie_parse_xml_actor (nfo_movie_t *movie, xmlNode *node)
{
  xmlNode *n;

  if (!movie || !node)
    return;

  n = nfo_get_node_xml_tree (node, "actor");
  while (n)
  {
    nfo_actor_t *actor;

    if (strcmp ((const char *) n->name, "actor"))
    {
      n = n->next;
      continue;
    }

    actor = nfo_actor_new ();
    nfo_xml_search_str (n, "name",  &actor->name);
    nfo_xml_search_str (n, "role",  &actor->role);
    nfo_xml_search_str (n, "thumb", &actor->thumb);

    nfo_movie_add_actor (movie, actor);
    n = n->next;
  }
}

static void
nfo_parse_xml_movie (nfo_t *nfo, const char *filename)
{
  xmlDocPtr doc;
  xmlNode *root, *movie, *fileinfo;
  nfo_movie_t *m;

  doc = nfo_get_xml_doc_from_file (filename);
  if (!doc)
    return;

  root = xmlDocGetRootElement (doc);
  if (!root)
    goto movie_err;

  movie = nfo_get_node_xml_tree (root, "movie");
  if (!movie)
    goto movie_err;

  m = nfo_movie_new ();

  fileinfo = nfo_get_node_xml_tree (movie, "fileinfo");
  nfo_movie_parse_xml_fileinfo (m, fileinfo);

  nfo_xml_search_str (movie, "title",         &m->title);
  nfo_xml_search_str (movie, "originaltitle", &m->original_title);
  nfo_xml_search_str (movie, "rating",        &m->rating);
  nfo_xml_search_str (movie, "year",          &m->year);
  nfo_xml_search_str (movie, "top250",        &m->top250);
  nfo_xml_search_str (movie, "votes",         &m->votes);
  nfo_xml_search_str (movie, "outline",       &m->outline);
  nfo_xml_search_str (movie, "plot",          &m->plot);
  nfo_xml_search_str (movie, "tagline",       &m->tagline);
  nfo_xml_search_str (movie, "runtime",       &m->runtime);
  nfo_xml_search_str (movie, "thumb",         &m->thumb);
  nfo_xml_search_str (movie, "fanart",        &m->fanart);
  nfo_xml_search_str (movie, "mpaa",          &m->mpaa);
  nfo_xml_search_str (movie, "playcount",     &m->playcount);
  nfo_xml_search_str (movie, "watched",       &m->watched);
  nfo_xml_search_str (movie, "id",            &m->id);
  nfo_xml_search_str (movie, "trailer",       &m->trailer);
  nfo_xml_search_str (movie, "genre",         &m->genre);
  nfo_xml_search_str (movie, "credits",       &m->credits);
  nfo_xml_search_str (movie, "director",      &m->director);
  nfo_xml_search_str (movie, "studioreal",    &m->studio);

  nfo_movie_parse_xml_actor (m, movie);

  nfo->type = NFO_MOVIE;
  nfo->movie = m;

 movie_err:
  xmlFreeDoc (doc);
}

void
nfo_parse_xml (nfo_t *nfo, const char *filename)
{
  char *s, *dir, *file, *nfo_file;

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
    nfo_parse_xml_movie (nfo, nfo_file);
    goto nfo_parse_xml_end;
  }

  nfo_file = nfo_file_exists (dir, "movie", "nfo");
  if (nfo_file)
  {
    nfo_parse_xml_movie (nfo, nfo_file);
    goto nfo_parse_xml_end;
  }

  nfo_file = nfo_file_exists (dir, "Movie", "nfo");
  if (nfo_file)
  {
    nfo_parse_xml_movie (nfo, nfo_file);
    goto nfo_parse_xml_end;
  }

 nfo_parse_xml_end:

  /* FanART: check for tbn file */
  if (nfo->type == NFO_MOVIE && nfo->movie)
  {
    char *tbn_file;

    tbn_file = nfo_file_exists (dir, file, "tbn");
    if (tbn_file)
      nfo->movie->fanart = strdup (tbn_file);
    NFREE (tbn_file);
  }

  NFREE (dir);
  NFREE (file);
  NFREE (nfo_file);
}
