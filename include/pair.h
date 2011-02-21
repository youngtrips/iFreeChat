/*
 * Author: youngtrips
 * Created Time:  2011-02-13
 * File Name: pair.h
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

#ifndef __PAIR_H
#define __PAIR_H

typedef struct pair_t {
	void *first;
	void *second;
}pair_t;

static inline void make_pair(pair_t *p, void *fir, void *sec) {
	p->first 	= fir;
	p->second 	= sec;
}

#endif
