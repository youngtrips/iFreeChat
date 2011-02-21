/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-28
 * File Name: encoding.c
 * Description: 
 *
 */

#include <string.h>

#include "gtk_common.h"

char *string_validate(const char *string, const char *codeset, char **encode)
{
	const char *pptr, *ptr;
	char *tstring, *cset;
	gsize rbytes, wbytes;

	*encode = NULL;
	tstring = NULL;
	if (!g_utf8_validate(string, -1, NULL) && codeset) {
		cset = NULL;
		ptr = codeset;
		do {
			if (*(pptr = ptr + strspn(ptr, ",;\x20\t")) == '\0')
				break;
			if (!(ptr = strpbrk(pptr, ",;\x20\t")))
				ptr = pptr + strlen(pptr);
			g_free(cset);
			cset = g_strndup(pptr, ptr - pptr);
			if (strcasecmp(cset, "utf-8") == 0)
				continue;
		} while (!(tstring = g_convert(string, -1, "utf-8", cset,
						 &rbytes, &wbytes, NULL)));
		*encode = cset;
	}

	return tstring;
}

