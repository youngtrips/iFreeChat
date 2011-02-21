/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-17
 * File Name: feiq.h
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

#ifndef __FEIQ_H
#define __FEIQ_H

#include "packet.h"
#include "msg.h"

#define CMD_BR_ENTRY	0x00000001UL
#define CMD_BR_EXIT		0x00000002UL
#define CMD_ANSENTRY	0x00000003UL
#define CMD_BR_ABSENCE	0x00000004UL

#define CMD_SENDMSG		0x00000020UL
#define CMD_RECVMSG		0x00000021UL
#define CMD_GPMSG		0x00000023UL

#define OPT_ABSENCE		0x00000100UL
#define OPT_UTF8		0x00800000UL

#define OPT_SENDCHECK	0x00000100UL




int feiq_build_packet(packet_t **pkt, const msg_t *msg);

int feiq_parse_packet(const packet_t *pkt, msg_t *msg);

int feiq_handle_msg(const msg_t *msg, void *user_data);

#endif
