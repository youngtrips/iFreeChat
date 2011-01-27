/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: test.c
 * Description: 
 *
 * You should have received a copy of the GNU General Public License
 * along with emesene; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "udp_socket.h"

static struct udp_socket_t usock;
int main() {

	init_udp_socket(&usock, "0.0.0.0", 9090);
	udp_start_listen(&usock);
	for(;;) {
		sleep(1);
	}
	return 0;
}
