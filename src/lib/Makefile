SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCES))
HEADS = $(wildcard *.h)
CC=gcc
CFLAGS=-g
LIBS=`pkg-config --cflags --libs glib-2.0 gtk+-2.0 libglade-2.0`
TARGETS = libifreechat.a

all: $(TARGETS)

.PHONY: all clean

%.o:%.c
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

$(TARGETS): $(OBJS)
		@echo $(SOURCES)
		@echo $(OBJS)
		ar rcs $@ $<

clean:
	rm $(TARGETS) $(OBJS)
