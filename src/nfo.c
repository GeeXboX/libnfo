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

#include "nfo.h"

struct nfo_actor_s {
    char *name;
    char *role;
    char *thumb;
};

struct nfo_movie_s {
    char *title;
    char *original_title;
    float rating;
    int year;
    int top250;
    float votes;
    char *outline;
    char *plot;
    char *tagline;
    int runtime;
    char *thumb;
    char *mpaa;
    int playcount;
    int watched;
    char *id;
    char *file_path;
    char *trailer;
    char *genre;
    char *credits;
    char *director;
    nfo_actor_t *actors; 
};

struct nfo_tvshow_s {
    char *title;
    char *episode_guide_url;
    float rating;
    int season;
    int episode;
    int displayseason;
    int displayepisode;
    char *plot;
    int runtime;
    char *mpaa;
    int watched;
    char *id;
    int tvdbid;
    char *genre;
    char *premiered;
    char *studio;
    char *sortorder;
    char *language;
    char episode_actor_source;
    nfo_actor_t *actors;
};

struct nfo_tvshow_episode_s {
    char *title;
    float rating;
    int season;
    int episode;
    char *plot;
    char *thumb;
    int playcount;
    char *credits;
    char *director;
    char *aired;
    nfo_actor_t *actors;
};

