libs=`pkg-config --cflags --libs glib-2.0 gtk+-2.0 libglade-2.0`
all:
	gcc -o test window.c test.c  $(libs)
