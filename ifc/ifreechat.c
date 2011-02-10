/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-10
 * File Name: ifreechat.c
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

#include "gtk_common.h"
#include "ifreechat.h"
#include "window.h"

#include "user.h"

ifreechat_t ifc;

void init_ifreechat(ifreechat_t *ifc) {

	init_dlist_node(&(ifc->pchatbox));
	init_dlist_node(&(ifc->gchatbox));
	init_dlist_node(&(ifc->ulist));
	init_dlist_node(&(ifc->glist));
	init_dlist_node(&(ifc->mlist));
}

int main(int argc, char *argv[]) {
	user_t *user;
	GtkTreeView *treeview;

	gtk_init(&argc, &argv);

	init_ifreechat(&ifc);

	init_window(&ifc, "glade/ui.glade");
	show_window(&ifc);

	treeview = ifc.main_window.contact_treeview;
	user = new_user("test1", "test1", "test1",
			"pixmaps/online.png", "172.16.18.1.1", "00:00:00:00:00", "test",
			"category1", "gbk");

	add_user_to_treeview(treeview, user);
	user = new_user("test2", "test2", "test2",
			"pixmaps/online.png", "172.16.18.1.2", "00:00:00:00:00", "test",
			"category1", "gbk");
	add_user_to_treeview(treeview, user);

	user = new_user("test3", "test3", "test3",
			"pixmaps/online.png", "172.16.18.1.3", "00:00:00:00:00", "test",
			"category2", "gbk");
	add_user_to_treeview(treeview, user);

	user = new_user("test4", "test3", "test3",
			"pixmaps/online.png", "172.16.18.1.4", "00:00:00:00:00", "test",
			"category3", "gbk");
	add_user_to_treeview(treeview, user);

	gtk_main();
	return 0;
}