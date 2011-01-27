OBJS=ifreechat.o \
	main_window.o \
	group.o \
	user.o


TARGET=ifreechat
CFLAGS=-g
LIBS=`pkg-config --cflags --libs glib-2.0 gtk+-2.0 libglade-2.0`
CC=gcc

$(TARGET): $(OBJS)
			 $(CC) -o $(TARGET) $(OBJS) $(LIBS) $(CFLAGS)

$(OBJS): %.o: %.c
		   $(CC) -c $(CFLAGS) $(LIBS) $< -o $@

clean:
	rm *.o -f

