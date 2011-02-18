SUBDIRS = feiq ui
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
				 -I ../include/net \
				 -I ../include/feiq \
				 -I ../include/ui

ifreechat_LDADD = ui/libmygui.a \
				  $(LIBXML_LIBS) $(GTK_LIBS) \
				  $(GLIB_LIBS) $(LIBGLADE_LIBS) \
				  ../lib/blowfish/libblowfish.a \
				  ../lib/mem_pool/libmempool.a \
				  ../lib/hash/libmyhash.a \
				  ../lib/net/libmynet.a \
				  ../lib/queue/libqueue.a \
				  ../lib/protocol/libprotocol.a \
				  feiq/libfeiq.a \
				  ../lib/encoding/libencoding.a \
				  -lpthread

ifreechat_SOURCES = ifreechat.c \
					group.c \
					user.c \
					handle_message.c \
					config.c \
					category.c
