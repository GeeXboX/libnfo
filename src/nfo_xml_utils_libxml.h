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

#ifndef NFO_XML_UTILS_LIBXML_H
#define NFO_XML_UTILS_LIBXML_H

xmlDocPtr nfo_get_xml_doc_from_file (const char *filename);

xmlNode *nfo_get_node_xml_tree (xmlNode *root, const char *prop);

xmlChar *nfo_get_prop_value_from_xml_tree (xmlNode *root, const char *prop);

#endif /* NFO_XML_UTILS_LIBXML_H */
