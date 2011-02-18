#!/usr/bin/env python

import socket

udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
addr = ("127.0.0.1", 9090)
msg = "1_lbt4_10#128#002481627512#0#0#0:1289671407:test:test-host:257:test-nickname\0heskell\0"

udp.sendto(msg, addr)
