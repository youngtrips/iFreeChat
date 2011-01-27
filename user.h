/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: user.h
 * Description: 
 *
 * You should have received a copy of the GNU General Public License
 * along with emesene; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __USER_H
#define __USER_H

#include "dlist.h"


struct user_t {
	char nickname[32];
	char avatar[64];
	char ip_addr[32];
	char mac_addr[32];
	char signature[128];
	char group_name[32];

	void *parent_iter;
	void *my_iter;

	struct dlist_t ulist_node;
	struct dlist_t glist_node;
};


#endif
