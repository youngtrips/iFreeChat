/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: global.h
 * Description: 
 *
 * You should have received a copy of the GNU General Public License
 * along with emesene; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "dlist.h"

struct iFreeChat_t {
	struct dlist_t group_list;
	struct dlist_t user_list;
};

#endif
