/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-01-27
 * File Name: global.h
 * Description: 
 */

#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "dlist.h"

struct iFreeChat_t {
	struct dlist_t group_list;
	struct dlist_t user_list;
};

#endif
