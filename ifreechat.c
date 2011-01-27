/*
 * Author: youngtrips
 * Created Time:  2011-01-26
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
#include "main_window.h"

#include "dlist.h"
#include "group.h"
#include "user.h"

static window_t win;

struct dlist_t glist;
struct dlist_t ulist;

int main(int argc, char *argv[]) {

	struct dlist_t *p;
	struct dlist_t *j;
	struct group_t *group;
	struct user_t *user;
	int i;
	
	gtk_init(&argc, &argv);
	init_group(&glist);

	init_main_window(&win, &glist, "glade/ui.glade");
	gtk_main();

	return 0;
}
