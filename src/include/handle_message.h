/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-16
 * File Name: handle_message.h
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

#ifndef __HANDLE_MESSAGE_H
#define __HANDLE_MESSAGE_H

#include "ifreechat.h"

int on_entry_callback(ifreechat_t *ifc, const void *msg);
int on_exit_callback(ifreechat_t *ifc, const void *msg);
int on_pchat_callback(ifreechat_t *ifc, const void *msg);
int on_gchat_callback(ifreechat_t *ifc, const void *msg);
int on_sendcheck_callback(ifreechat_t *ifc, const void *msg);

void *process_message_loop(ifreechat_t *ifc);

#endif
