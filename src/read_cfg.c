/*
 * Author: youngtrips
 * Created Time:  2011-02-10
 * File Name: read_cfg.c
 * Description: 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "read_cfg.h"

int read_cfg(const char *cfgfile, ifreechat_t *ifc) {

	xmlDocPtr doc;
	xmlNodePtr curNode;
	xmlNodePtr p;
	xmlChar *szKey;

	doc = xmlReadFile(cfgfile,
			"UTF-8", XML_PARSE_RECOVER);
	if (doc == NULL) {
		return -1;
	}
	curNode = xmlDocGetRootElement(doc);

	if (NULL == curNode) {
		xmlFreeDoc(doc);
		return -1;
	}

	curNode = curNode->xmlChildrenNode;
	while(curNode != NULL) {
		if (!xmlStrcmp(curNode->name, (const xmlChar*)"user")) {
			p = curNode->xmlChildrenNode;
			while(p != NULL) {
				if (p->type == XML_ELEMENT_NODE) {
					szKey = xmlNodeGetContent(p);
					if (!xmlStrcmp(p->name, (const xmlChar*)"nickname")) {
						strcpy(ifc->nickname, szKey);
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"username")) {
						strcpy(ifc->username, szKey);
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"hostname")) {
						strcpy(ifc->hostname, szKey);
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"avatar_type")) {
						ifc->avatar_type = atoi(szKey);
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"avatar_id")) {
						ifc->avatar_id = atoi(szKey);
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"custom_avatar")) {
						strcpy(ifc->custom_avatar, szKey);
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"signature")) {
						strcpy(ifc->signature, szKey);
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"category")) {
						strcpy(ifc->category, szKey);
					}
					xmlFree(szKey);
				}
				p = p->next;
			}
		} else if (!xmlStrcmp(curNode->name, (const xmlChar*)"network")) {
			p = curNode->xmlChildrenNode;
			while(p != NULL) {
				if (p->type == XML_ELEMENT_NODE) {
					szKey = xmlNodeGetContent(p);
					if (!xmlStrcmp(p->name, (const xmlChar*)"listen_ip")) {
						strcpy(ifc->ipaddr, szKey);
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"multicast_ip")) {
						strcpy(ifc->multicast_ip, szKey);
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"port")) {
						ifc->port = atoi(szKey);
					}
					xmlFree(szKey);
				}
				p = p->next;
			}
		}
		curNode = curNode->next;
	}
	xmlFreeDoc(doc);
	printf("ok...\n");
	return 0;
}



