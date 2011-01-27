OBJS=ifreechat.o \
	main_window.o \
	group.o \
	user.o


TARGET=ifreechat
CFLAGS=-g
LIBS=`pkg-config --cflags --libs glib-2.0 gtk+-2.0 libglade-2.0`
CC=gcc
INC=-I ./lib

$(TARGET): $(OBJS)
			 $(CC) -o $(TARGET) $(OBJS) $(LIBS) $(CFLAGS) $(INC)

$(OBJS): %.o: %.c
		   $(CC) -c $(CFLAGS) $(LIBS) $(INC) $< -o $@

clean:
	rm *.o -f

