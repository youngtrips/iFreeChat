/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: group.h
 * Description: 
 *
 * You should have received a copy of the GNU General Public License
 * along with emesene; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __GROUP_H
#define __GROUP_H

#include "dlist.h"

struct group_t {
	char group_name[32];

	struct dlist_t mlist_head;
	struct dlist_t glist_node;

	void *my_iter;
};


enum {
	SEARCH_BY_NAME,
	SEARCH_BY_IPV4,
	SEARCH_BY_MAC
};


#endif