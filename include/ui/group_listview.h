/*
 * Author: youngtrips
 * Created Time:  2011-02-13
 * File Name: group_listview.h
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

#ifndef __GROUP_LISTVIEW_H
#define __GROUP_LISTVIEW_H

#include "gtk_common.h"
#include "ifreechat.h"

void group_treeview_ondoubleclicked(GtkTreeView *tree_view,
		GtkTreePath *path, GtkTreeViewColumn *col, gpointer data);

#endif
