/*
 * Author: youngtrips
 * Created Time:  2011-02-10
 * File Name: process_msg.h
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

#ifndef __PROCESS_MSG_H
#define __PROCESS_MSG_H

#include "ifreechat.h"
#include "msg.h"

#define CMD_BR_ENTRY	0x00000001UL
#define CMD_BR_EXIT		0x00000002UL
#define CMD_ANSENTRY	0x00000003UL
#define CMD_BR_ABSENCE	0x00000004UL

#define CMD_SENDMSG		0x00000020UL
#define CMD_RECVMSG		0x00000021UL

#define OPT_ABSENCE		0x00000100UL
#define OPT_UTF8		0x00800000UL

#define OPT_SENDCHECK	0x00000100UL



#define MAXN_FUNC 0xFF

typedef int (*handle_msg_func)(ifreechat_t *ifc, msg_t *msg);


#endif
