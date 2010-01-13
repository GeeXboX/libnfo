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

#include <string.h>
#include <stdlib.h>

#include "nfo_osdep.h"

#ifdef NEED_OSDEP_STRNDUP
char *
strndup (const char *s, size_t n)
{
  char *res;
  size_t length;

  length = strlen (s);
  if (length > n)
    length = n;

  res = malloc (length + 1);
  if (!res)
    return NULL;

  memcpy (res, s, length);
  res[length] = '\0';

  return res;
}
#endif /* NEED_OSDEP_STRNDUP */
