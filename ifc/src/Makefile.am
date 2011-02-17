SUBDIRS = net protocol
bin_PROGRAMS = ifreechat

ifreechat_CPPFLAGS=$(LIBXML_CFLAGS) $(GTK_CFLAGS) \
				   $(GLIB_CFLAGS) $(LIBGLADE_CFLAGS) \
				 -I ../include -I ../include/mem_pool \
				 -I ../include/net -I ../include/net \
				 -I ../include/protocol

ifreechat_LDADD = $(LIBXML_LIBS) $(GTK_LIBS) \
				  $(GLIB_LIBS) $(LIBGLADE_CFLAGS) \
				  ../lib/libifreechat.a \
				  net/libmynet.a \
				  protocol/libprotocol.a \
				  -lpthread

ifreechat_SOURCES = ifreechat.c \
					group.c \
					user.c \
					handle_message.c
