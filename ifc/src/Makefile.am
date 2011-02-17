SUBDIRS = 
bin_PROGRAMS = ifreechat

ifreechat_CPPFLAGS=$(LIBXML_CFLAGS) $(GTK_CFLAGS) \
				   $(GLIB_CFLAGS) $(LIBGLADE_CFLAGS) \
				 -I ../include \
				 -I ../include/mem_pool \
				 -I ../include/blowfish \
				 -I ../include/hash \
				 -I ../include/encoding \
				 -I ../include/protocol \
				 -I ../include/queue \
				 -I ../include/net

ifreechat_LDADD = $(LIBXML_LIBS) $(GTK_LIBS) \
				  $(GLIB_LIBS) $(LIBGLADE_CFLAGS) \
				  ../lib/blowfish/libblowfish.a \
				  ../lib/encoding/libencoding.a \
				  ../lib/mem_pool/libmempool.a \
				  ../lib/hash/libmyhash.a \
				  ../lib/net/libmynet.a \
				  ../lib/queue/libqueue.a \
				  ../lib/protocol/libprotocol.a \
				  -lpthread

ifreechat_SOURCES = ifreechat.c \
					group.c \
					user.c \
					handle_message.c
