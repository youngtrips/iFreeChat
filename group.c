/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: group.c
 * Description: 
 *
 * You should have received a copy of the GNU General Public License
 * along with emesene; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "dlist.h"
#include "group.h"
#include "user.h"

void init_group(struct dlist_t *glist) {
	struct group_t *group;
	init_dlist_node(glist);
	group = (struct group_t*)malloc(sizeof(struct group_t));
	if (group == NULL) {
		fprintf(stderr, "malloc() error: %s\n", strerror(errno));
		return;
	}
	strcpy(group->group_name, "No Group");
	init_dlist_node(&(group->glist_node));
	init_dlist_node(&(group->mlist_head));

	dlist_add(&(group->glist_node), glist);
}

void add_group(struct dlist_t *glist, struct group_t *group) {
}

void group_add_user(struct dlist_t *glist, struct user_t *user) {
	struct dlist_t *pos;
	struct group_t *group;

	dlist_foreach(pos, glist) {
		group = (struct group_t*)dlist_entry(pos, struct group_t, glist_node);
		if (!strcmp(group->group_name, user->group_name)) {
			dlist_add(&(user->glist_node), &(group->mlist_head));

			/* update treeview model */

			return;
		}
	}

	group = (struct group_t*)malloc(sizeof(struct group_t));
	strcpy(group->group_name, user->group_name);
	init_dlist_node(&(group->mlist_head));
	init_dlist_node(&(group->glist_node));

	dlist_add(&(group->glist_node), glist);

	/* update treeview model */

}

struct user_t *group_find_user(struct dlist_t *glist, int type, const char *key) {
	struct group_t *group;
	struct user_t *user;
	struct dlist_t *i;
	struct dlist_t *j;
	char *p;

	dlist_foreach(i, glist) {
		group = (struct group_t*)dlist_entry(i, struct group_t, glist_node);
		
		dlist_foreach(j, &(group->mlist_head)) {
			user = (struct user_t*)dlist_entry(j, struct user_t, glist_node);
			switch(type) {
				case SEARCH_BY_NAME:
					p = user->nickname; break;
				case SEARCH_BY_IPV4:
					p = user->ip_addr; 	break;
				case SEARCH_BY_MAC:
					p = user->mac_addr;	break;
			}
			if (!strcmp(p, key)) {
				return user;
			}
		}
	}
	return NULL;
}

















