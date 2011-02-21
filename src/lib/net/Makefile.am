noinst_LIBRARIES = libmynet.a

AM_CPPFLAGS = $(GTK_FLAGS) $(GLIB2_CFLAGS) \
						   $(LIBGLADE_CFLAGS) -g -Wall \
						 -I ../../include \
						 -I ../../include/blowfish \
						 -I ../../include/mem_pool \
						 -I ../../include/net \
						 -I ../../include/protocol \
						 -I ../../include/queue

libmynet_a_LIBADD = ../queue/libqueue.a

libmynet_a_SOURCES = msg_queue.c \
					 packet.c \
					 udp_socket.c \
					 utils.c
