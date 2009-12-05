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

#include <stdio.h>

#include <nfo.h>

#define PRINT_MOVIE_TAG(str,field)                                      \
  if (nfo_movie_get (movie, field))                                     \
    printf ("  %s: %s\n", str, nfo_movie_get (movie, field))

#define PRINT_EPISODE_TAG(str,field)                                    \
  if (nfo_tvshow_episode_get (episode, field))                          \
    printf ("  %s: %s\n", str, nfo_tvshow_episode_get (episode, field))

#define PRINT_TVSHOW_TAG(str,field)                                     \
  if (nfo_tvshow_get (tvshow, field))                                   \
    printf ("    %s: %s\n", str, nfo_tvshow_get (tvshow, field))

static void
display_actor (nfo_actor_t *actor)
{
  char *name, *role, *thumb;

  if (!actor)
    return;

  name  = nfo_actor_get (actor, NFO_ACTOR_NAME);
  role  = nfo_actor_get (actor, NFO_ACTOR_ROLE);
  thumb = nfo_actor_get (actor, NFO_ACTOR_THUMB);

  printf ("    * %s as %s (see %s)\n", name, role, thumb);
}

static void
display_video_stream (nfo_stream_video_t *video, int id)
{
  char *width, *height, *codec, *format_info, *duration;
  char *bitrate, *bitrate_mode, *bitrate_max;
  char *container, *codec_id, *codec_info, *scan_type, *aspect;

  if (!video)
    return;

  width        = nfo_video_stream_get (video, NFO_VIDEO_WIDTH);
  height       = nfo_video_stream_get (video, NFO_VIDEO_HEIGHT);
  codec        = nfo_video_stream_get (video, NFO_VIDEO_CODEC);
  format_info  = nfo_video_stream_get (video, NFO_VIDEO_FORMAT_INFO);
  duration     = nfo_video_stream_get (video, NFO_VIDEO_DURATION);

  bitrate      = nfo_video_stream_get (video, NFO_VIDEO_BITRATE);
  bitrate_mode = nfo_video_stream_get (video, NFO_VIDEO_BITRATE_MODE);
  bitrate_max  = nfo_video_stream_get (video, NFO_VIDEO_BITRATE_MAX);

  container    = nfo_video_stream_get (video, NFO_VIDEO_CONTAINER);
  codec_id     = nfo_video_stream_get (video, NFO_VIDEO_CODEC_ID);
  codec_info   = nfo_video_stream_get (video, NFO_VIDEO_CODEC_INFO);
  scan_type    = nfo_video_stream_get (video, NFO_VIDEO_SCAN_TYPE);
  aspect       = nfo_video_stream_get (video, NFO_VIDEO_ASPECT);

  printf ("    * Stream #%d: %sx%s %s %s (%s) in %s\n",
          id, width, height, codec, format_info, codec_id, container);
  printf ("      - Duration: %s\n", duration);
  printf ("      - Birate: %s (Max: %s - Mode: %s)\n",
          bitrate, bitrate_max, bitrate_mode);
  printf ("      - Codec: %s, using %s scan with %s aspect ratio\n",
          codec_info, scan_type, aspect);
}

static void
display_audio_stream (nfo_stream_audio_t *audio, int id)
{
  char *lang, *codec, *channels, *bitrate;

  if (!audio)
    return;

  lang     = nfo_audio_stream_get (audio, NFO_AUDIO_LANG);
  codec    = nfo_audio_stream_get (audio, NFO_AUDIO_CODEC);
  channels = nfo_audio_stream_get (audio, NFO_AUDIO_CHANNELS);
  bitrate  = nfo_audio_stream_get (audio, NFO_AUDIO_BITRATE);

  printf ("    * Stream #%d: %s - %s channels at %s kbps (%s)\n",
          id, codec, channels, bitrate, lang);
}

static void
display_sub_stream (nfo_stream_sub_t *sub, int id)
{
  char *lang;

  if (!sub)
    return;

  lang = nfo_sub_stream_get (sub, NFO_SUB_LANG);

  printf ("    * Stream #%d: %s\n", id, lang);
}

static void
display_movie (nfo_t *nfo)
{
  nfo_movie_t *movie;
  int i, c;

  if (!nfo)
    return;

  movie = nfo_get_movie (nfo);
  if (!movie)
    return;

  PRINT_MOVIE_TAG ("Title",          NFO_MOVIE_TITLE);
  PRINT_MOVIE_TAG ("Original Title", NFO_MOVIE_ORIGINAL_TITLE);
  PRINT_MOVIE_TAG ("Rating",         NFO_MOVIE_RATING);
  PRINT_MOVIE_TAG ("Year",           NFO_MOVIE_YEAR);
  PRINT_MOVIE_TAG ("Top250",         NFO_MOVIE_TOP250);
  PRINT_MOVIE_TAG ("Votes",          NFO_MOVIE_VOTES);
  PRINT_MOVIE_TAG ("Outline",        NFO_MOVIE_OUTLINE);
  PRINT_MOVIE_TAG ("Plot",           NFO_MOVIE_PLOT);
  PRINT_MOVIE_TAG ("Tagline",        NFO_MOVIE_TAGLINE);
  PRINT_MOVIE_TAG ("Runtime",        NFO_MOVIE_RUNTIME);
  PRINT_MOVIE_TAG ("Thumb",          NFO_MOVIE_THUMB);
  PRINT_MOVIE_TAG ("FanArt",         NFO_MOVIE_FAN_ART);
  PRINT_MOVIE_TAG ("MPAA",           NFO_MOVIE_MPAA);
  PRINT_MOVIE_TAG ("PlayCount",      NFO_MOVIE_PLAY_COUNT);
  PRINT_MOVIE_TAG ("Watched",        NFO_MOVIE_WATCHED);
  PRINT_MOVIE_TAG ("MovieID",        NFO_MOVIE_ID);
  PRINT_MOVIE_TAG ("Trailer",        NFO_MOVIE_TRAILER);
  PRINT_MOVIE_TAG ("Genre",          NFO_MOVIE_GENRE);
  PRINT_MOVIE_TAG ("Credits",        NFO_MOVIE_CREDITS);
  PRINT_MOVIE_TAG ("Director",       NFO_MOVIE_DIRECTOR);
  PRINT_MOVIE_TAG ("Studio",         NFO_MOVIE_STUDIO);

  c = nfo_movie_get_actors_count (movie);
  if (c)
    printf ("  Actors:\n");
  for (i = 0; i < c; i++)
  {
    nfo_actor_t *actor;

    actor = nfo_movie_get_actor (movie, i);
    display_actor (actor);
  }

  c = nfo_movie_get_video_streams_count (movie);
  if (c)
    printf ("  Video Streams:\n");
  for (i = 0; i < c; i++)
  {
    nfo_stream_video_t *video;

    video = nfo_movie_get_video_stream (movie, i);
    display_video_stream (video, i);
  }

  c = nfo_movie_get_audio_streams_count (movie);
  if (c)
    printf ("  Audio Streams:\n");
  for (i = 0; i < c; i++)
  {
    nfo_stream_audio_t *audio;

    audio = nfo_movie_get_audio_stream (movie, i);
    display_audio_stream (audio, i);
  }

  c = nfo_movie_get_sub_streams_count (movie);
  if (c)
    printf ("  Subtitle Streams:\n");
  for (i = 0; i < c; i++)
  {
    nfo_stream_sub_t *sub;

    sub = nfo_movie_get_sub_stream (movie, i);
    display_sub_stream (sub, i);
  }
}

static void
display_show (nfo_tvshow_t *tvshow)
{
  if (!tvshow)
    return;

  printf ("  Belonging TVShow:\n");
  PRINT_TVSHOW_TAG ("Title",          NFO_TVSHOW_TITLE);
  PRINT_TVSHOW_TAG ("Rating",         NFO_TVSHOW_RATING);
  PRINT_TVSHOW_TAG ("Season",         NFO_TVSHOW_SEASON);
  PRINT_TVSHOW_TAG ("Episode",        NFO_TVSHOW_EPISODE);
  PRINT_TVSHOW_TAG ("DispalySeason",  NFO_TVSHOW_DISPLAY_SEASON);
  PRINT_TVSHOW_TAG ("DisplayEpisode", NFO_TVSHOW_DISPLAY_EPISODE);
  PRINT_TVSHOW_TAG ("GuideURL",       NFO_TVSHOW_EPISODE_GUIDE_URL);
  PRINT_TVSHOW_TAG ("Plot",           NFO_TVSHOW_PLOT);
  PRINT_TVSHOW_TAG ("MPAA",           NFO_TVSHOW_MPAA);
  PRINT_TVSHOW_TAG ("Watched",        NFO_TVSHOW_WATCHED);
  PRINT_TVSHOW_TAG ("Genre",          NFO_TVSHOW_GENRE);
  PRINT_TVSHOW_TAG ("Premiered",      NFO_TVSHOW_PREMIERED);
  PRINT_TVSHOW_TAG ("Studio",         NFO_TVSHOW_STUDIO);
  PRINT_TVSHOW_TAG ("Fan Art",        NFO_TVSHOW_FANART);
  PRINT_TVSHOW_TAG ("Fan Art Header", NFO_TVSHOW_FANART_HEADER);
  PRINT_TVSHOW_TAG ("Fan Art Cover",  NFO_TVSHOW_FANART_COVER);
}

static void
display_tvshow (nfo_t *nfo)
{
  nfo_tvshow_episode_t *episode;
  nfo_tvshow_t *tvshow;
  int i, c;

  if (!nfo)
    return;

  episode = nfo_get_tvshow_episode (nfo);
  if (!episode)
    return;

  PRINT_EPISODE_TAG ("Title",     NFO_TVSHOW_EPISODE_TITLE);
  PRINT_EPISODE_TAG ("Rating",    NFO_TVSHOW_EPISODE_RATING);
  PRINT_EPISODE_TAG ("Season",    NFO_TVSHOW_EPISODE_SEASON);
  PRINT_EPISODE_TAG ("Episode",   NFO_TVSHOW_EPISODE_EPISODE);
  PRINT_EPISODE_TAG ("Plot",      NFO_TVSHOW_EPISODE_PLOT);
  PRINT_EPISODE_TAG ("Thumb",     NFO_TVSHOW_EPISODE_THUMB);
  PRINT_EPISODE_TAG ("FanArt",    NFO_TVSHOW_EPISODE_FANART);
  PRINT_EPISODE_TAG ("FanArt S.", NFO_TVSHOW_EPISODE_FANART_SEASON);
  PRINT_EPISODE_TAG ("PlayCount", NFO_TVSHOW_EPISODE_PLAY_COUNT);
  PRINT_EPISODE_TAG ("Credits",   NFO_TVSHOW_EPISODE_CREDITS);
  PRINT_EPISODE_TAG ("Director",  NFO_TVSHOW_EPISODE_DIRECTOR);
  PRINT_EPISODE_TAG ("Aired",     NFO_TVSHOW_EPISODE_AIRED);
  PRINT_EPISODE_TAG ("Votes",     NFO_TVSHOW_EPISODE_VOTES);

  tvshow = nfo_tvshow_episode_get_show (episode);
  display_show (tvshow);

  c = nfo_tvshow_episode_get_actors_count (episode);
  if (c)
    printf ("  Actors:\n");
  for (i = 0; i < c; i++)
  {
    nfo_actor_t *actor;

    actor = nfo_tvshow_episode_get_actor (episode, i);
    display_actor (actor);
  }

  c = nfo_tvshow_episode_get_video_streams_count (episode);
  if (c)
    printf ("  Video Streams:\n");
  for (i = 0; i < c; i++)
  {
    nfo_stream_video_t *video;

    video = nfo_tvshow_episode_get_video_stream (episode, i);
    display_video_stream (video, i);
  }

  c = nfo_tvshow_episode_get_audio_streams_count (episode);
  if (c)
    printf ("  Audio Streams:\n");
  for (i = 0; i < c; i++)
  {
    nfo_stream_audio_t *audio;

    audio = nfo_tvshow_episode_get_audio_stream (episode, i);
    display_audio_stream (audio, i);
  }

  c = nfo_tvshow_episode_get_sub_streams_count (episode);
  if (c)
    printf ("  Subtitle Streams:\n");
  for (i = 0; i < c; i++)
  {
    nfo_stream_sub_t *sub;

    sub = nfo_tvshow_episode_get_sub_stream (episode, i);
    display_sub_stream (sub, i);
  }
}

int
main (int argc, char **argv)
{
  char *filename;
  nfo_type_t type;
  nfo_t *nfo;

  if (argc < 2)
    return 1;

  filename = argv[1];
  if (!filename)
    return 1;

  nfo = nfo_init (filename);
  if (!nfo)
    return 1;

  type = nfo_get_type (nfo);
  switch (type)
  {
  case NFO_MOVIE:
    display_movie (nfo);
    break;
  case NFO_TVSHOW:
    display_tvshow (nfo);
    break;
  default:
    printf ("No NFO file available or unsupported type.\n");
    break;
  }

  nfo_free (nfo);
  return 0;
}
