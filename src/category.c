/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-14
 * File Name: category.c
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
#include "category.h"

category_t *new_category(const char *name) {
	category_t *cat;

	cat = (category_t*)malloc(sizeof(category_t));
	if (cat == NULL) 
		return NULL;

	strcpy(cat->name, name);
	cat->count = 0;
	init_dlist_node(&(cat->cnode));

	return cat;
}


