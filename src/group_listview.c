/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-12
 * File Name: group_listview.c
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

#include "gtk_common.h"
#include "group.h"

enum {
	PIXBUF_COL,
	TITLE_COL,
	URI_COL,
	COL_NUM
};

void add_group_to_listview(GtkTreeView *listview, group_t *gp) {
}

void del_group_from_listview(GtkTreeView *listview, group_t *gp) {
}

void group_listview_ondoubleclicked(GtkTreeView *listview,
		GtkTreePath *path, GtkTreeViewColumn *col, gpointer data) {
}

