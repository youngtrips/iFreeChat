lib_LTLIBRARIES = libifreechat.la

libifreechat_la_CPPFLAGS = $(GTK_FLAGS) $(GLIB2_CFLAGS) \
						   $(LIBGLADE_CFLAGS) -g -Wall \
						 -I ../include -I ../include/blowfish \
						 -I ../include/mem_pool

libifreechat_la_LDFLAGS = $(GTK_LIBS) $(GLIB2_CFLAGS) \
						  $(LIBGLADE_LIBS) -lpthread

libifreechat_la_SOURCES = blowfish/blowfish.c \
						  mem_pool/mem_pool.c \
						  encoding.c \
						  hash.c \
						  queue.c
