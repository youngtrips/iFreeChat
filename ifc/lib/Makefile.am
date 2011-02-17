noinst_LIBRARIES = libifreechat.a

AM_CPPFLAGS = $(GTK_FLAGS) $(GLIB2_CFLAGS) \
						   $(LIBGLADE_CFLAGS) -g -Wall \
						 -I ../include \
						 -I ../include/blowfish \
						 -I ../include/mem_pool

libifreechat_a_LIBADD = 

libifreechat_a_SOURCES = blowfish/blowfish.c \
						  mem_pool/mem_pool.c \
						  encoding.c \
						  hash.c
