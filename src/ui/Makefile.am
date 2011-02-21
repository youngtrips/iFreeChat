noinst_LIBRARIES = libmygui.a

AM_CPPFLAGS = $(LIBXML_CFLAGS) $(GTK_CFLAGS) \
			  $(GLIB_CFLAGS) $(LIBGLADE_CFLAGS) \
			  -I ../../include \
			  -I ../../include/mem_pool \
			  -I ../../include/blowfish \
			  -I ../../include/hash \
			  -I ../../include/encoding \
			  -I ../../include/protocol \
			  -I ../../include/queue \
			  -I ../../include/net \
			  -I ../../include/ui

libmygui_a_LIBADD =

libmygui_a_SOURCES = window.c \
				   user_treeview.c \
				   listview.c \
				   group_listview.c \
				   chatbox.c \
				   pchatbox.c \
				   gchatbox.c \
				   emotion_box.c
