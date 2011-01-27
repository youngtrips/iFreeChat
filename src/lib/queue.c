/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: queue.c
 * Description: 
 *
 * You should have received a copy of the GNU General Public License
 * along with emesene; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "queue.h"


struct queue_t *queue_init(size_t size) {
	struct queue_t *q;

	q = (struct queue_t*)malloc(sizeof(struct queue_t));
	if (q == NULL) {
		return NULL;
	}

	q->data = (void*)malloc(sizeof(void*) * size);
	if (q->data == NULL) {
		free(q);
		return NULL;
	}

	q->size = size;
	q->head = 0;
	q->tail = 0;
	return q;
}

void queue_destroy(struct queue_t *q) {
	if (q) {
		if (q->data) 
			free(q->data);
		free(q);
	}
}


int queue_push(struct queue_t *q, const void *data) {
	if ((q->tail + 1) % q->size == q->head) {
		return -1;
	}

	q->data[q->tail] = (void*)data;
	q->tail = (q->tail + 1) % q->size;
	return 0;
}

void *queue_pop(struct queue_t *q) {
	void *res;
	if (q->head == q->tail)
		return NULL;

	res = q->data[q->head];
	q->head = (q->head + 1) % q->size;
	return  res;
}

