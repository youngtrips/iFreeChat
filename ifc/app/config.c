/*
 * Author: youngtrips
 * Created Time:  2011-02-10
 * File Name: config.c
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

#include "group.h"
#include "config.h"

int read_cfg(ifreechat_t *ifc) {

	xmlDocPtr doc;
	xmlNodePtr curNode;
	xmlNodePtr p;
	xmlChar *szKey;

	char gpname[128];
	char gpinfo[128];
	uint32_t gpid;

	group_t *gp;

	doc = xmlReadFile(ifc->cfgfile,
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
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"mac")) {
						strcpy(ifc->macaddr, szKey);
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"multicast_ip")) {
						strcpy(ifc->multicast_ip, szKey);
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"port")) {
						ifc->port = atoi(szKey);
					}
					xmlFree(szKey);
				}
				p = p->next;
			}
		} else if (!xmlStrcmp(curNode->name, (const xmlChar*)"group")) {
			p = curNode->xmlChildrenNode;
			while(p != NULL) {
				if (p->type == XML_ELEMENT_NODE) {
					szKey = xmlNodeGetContent(p);
					if (!xmlStrcmp(p->name, (const xmlChar*)"name")) {
						strcpy(gpname, szKey);
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"id")) {
						gpid = strtoul(szKey, NULL, 16);
					} else if (!xmlStrcmp(p->name, (const xmlChar*)"info")) {
						strcpy(gpinfo, szKey);
					}
				}
				p = p->next;
			}
			gp = new_group(gpname, gpinfo, gpid);
			if (gp) {
				dlist_add_tail(&(gp->gnode), &(ifc->glist));
			}
		}
		curNode = curNode->next;
	}
	xmlFreeDoc(doc);
	printf("load config ok...\n");
	return 0;
}


int update_cfg(ifreechat_t *ifc, const char *category, 
		const char *name, const char *value) {

	xmlDocPtr doc;
	xmlNodePtr curNode;
	xmlNodePtr p;
	xmlChar *szKey;
	int flag;

	printf("category: [%s]\n", category);
	printf("name: [%s] --> value:[%s]\n", name, value);
	doc = xmlReadFile(ifc->cfgfile,
			"UTF-8", XML_PARSE_RECOVER);
	if (doc == NULL) {
		return -1;
	}
	curNode = xmlDocGetRootElement(doc);

	if (NULL == curNode) {
		xmlFreeDoc(doc);
		return -1;
	}

	flag = 0;
	curNode = curNode->xmlChildrenNode;
	while(curNode != NULL) {
		if (!xmlStrcmp(curNode->name, (const xmlChar*)category)) {
			p = curNode->xmlChildrenNode;
			while(p != NULL) {
				if (p->type == XML_ELEMENT_NODE) {
					if (!xmlStrcmp(p->name, (const xmlChar*)name)) {
						flag = 1;
						xmlNodeSetContent(p, (const xmlChar*)value);
						break;
					} 
				}
				p = p->next;
			}
		}
		if (flag == 1)
			break;
		curNode = curNode->next;
	}
	xmlSaveFormatFile(ifc->cfgfile, doc, 1);
	xmlFreeDoc(doc);
	printf("update config ok...\n");
	return 0;
}


