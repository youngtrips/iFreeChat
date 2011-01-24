/*
 * Author: youngtrips
 * Created Time:  2011-01-24
 * File Name: protocol.h
 * Description: 
 */

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

typedef struct msg_t {
	uint16_t buf_size;
	uint8_t buf[1];
}msg_t;


#endif
