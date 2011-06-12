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

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

#include "nfo_xml_utils_libxml.h"

xmlDocPtr
nfo_get_xml_doc_from_file (const char *filename)
{
  xmlDocPtr doc;

  if (!filename)
    return NULL;

  doc =
    xmlReadFile (filename, NULL,
                 XML_PARSE_RECOVER | XML_PARSE_NOERROR | XML_PARSE_NOWARNING);
  return doc;
}

xmlNode *
nfo_get_node_xml_tree (xmlNode *root, const char *prop)
{
  xmlNode *n, *children_node;

  if (!root || !prop)
    return NULL;

  for (n = root; n; n = n->next)
  {
    if (n->type == XML_ELEMENT_NODE
        && !xmlStrcmp ((unsigned char *) prop, n->name))
      return n;

    children_node = nfo_get_node_xml_tree (n->children, prop);
    if (children_node)
      return children_node;
  }

  return NULL;
}

xmlChar *
nfo_get_prop_value_from_xml_tree (xmlNode *root, const char *prop)
{
  xmlNode *node;

  node = nfo_get_node_xml_tree (root, prop);
  if (!node)
    return NULL;

  return xmlNodeGetContent (node);
}
