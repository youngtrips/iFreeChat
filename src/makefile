OBJS=ifreechat.o \
	 group.o \
	 user.o \
	 window.o \
	 config.o \
	 pchatbox.o \
	 udp_socket.o \
	 utils.o \
	 broadcast.o \
	 process_msg.o \
	 encoding.o \
	 emotion_box.o \
	 group_listview.o \
	 listview.o \
	 treeview.o \
	 gchatbox.o \
	 chatbox.o \
	 category.o \
	 blowfish.o

CC=gcc
TARGET=ifreechat
CFLAGS=`pkg-config --cflags glib-2.0 gdk-x11-2.0 gtk+-2.0 libglade-2.0` -I /usr/include/libxml2 -g
LIBS=`pkg-config --libs glib-2.0 gdk-x11-2.0 gtk+-2.0 libglade-2.0` -lxml2 -lpthread -lssl -lcrypto

$(TARGET): $(OBJS)
			 $(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LIBS)

$(OBJS): %.o: %.c
		   $(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o -f
