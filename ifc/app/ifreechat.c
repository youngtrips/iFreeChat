/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-16
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

#include "ifreechat.h"

static int init_freechat(ifreechat_t **ifc) {
	return 0;
}

static void destroy_freechat(ifreechat_t *ifc) {
}

int main() {

	ifreechat_t *ifc;

	if (init_freechat(&ifc) < 0)
		return 1;

	destroy_freechat(ifc);
	return 0;
}
