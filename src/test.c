#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

int main() {

	xmlDocPtr doc;
	xmlNodePtr curNode;
	xmlNodePtr p;
	xmlChar *szKey;

	doc = xmlReadFile("ifreechat.xml",
			"UTF-8", XML_PARSE_RECOVER);
	if (doc == NULL) {
		return 1;
	}
	curNode = xmlDocGetRootElement(doc);

	if (NULL == curNode) {
		xmlFreeDoc(doc);
		return 1;
	}
	printf("root name: %s\n", curNode->name);

	curNode = curNode->xmlChildrenNode;
	while(curNode != NULL) {
		if (!xmlStrcmp(curNode->name, (const xmlChar*)"user")) {
			p = curNode->xmlChildrenNode;
			while(p != NULL) {
				if (p->type == XML_ELEMENT_NODE) {
					szKey = xmlNodeGetContent(p);
					printf("%s: %s\n", p->name, szKey);
					xmlFree(szKey);
				}
				p = p->next;
			}
		}
		curNode = curNode->next;
	}
	xmlFreeDoc(doc);
	return 0;
}
