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

#endif /* NFO_H */
