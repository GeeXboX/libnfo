/*
 * GeeXboX libnfo: a .NFO file reader/writer.
 * Copyright (C) 2009-2010 Benjamin Zores <ben@geexbox.org>
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

#ifndef NFO_OSDEP_H
#define NFO_OSDEP_H

#ifdef NEED_OSDEP_STRNDUP
char * strndup (const char *s, size_t n);
#endif /* NEED_OSDEP_STRNDUP */

#endif /* NFO_OSDEP_H */
