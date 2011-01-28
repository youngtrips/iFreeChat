CFLAGS=-g
LIBS=`pkg-config --cflags --libs glib-2.0 gtk+-2.0 libglade-2.0` -lpthread
CC=gcc

SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCES))
HEADERS = $(wildcard *.h)

SUB_DIR = lib
SOURCES_SUB = $(wildcard $(SUB_DIR)/*.c)
OBJS_SUB = $(patsubst %.c, %.o, $(SOURCES_SUB))
INCLUDE = -I./lib
TARGETS = ifreechat

all: SUB $(TARGETS)

.PHONY: all clean

%.o:%.c
	$(CC) -c $(CFLAGS) $(LIBS) $(INCLUDE) $< -o $@
	
$(TARGETS): $(OBJS)
	@echo "sources_lib: "$(SOURCES_SUB)
	@echo "source: "$(SOURCES)
	@echo "objs: "$(OBJS)
	@echo "objs_lib: "$(OBJS_SUB)
	$(CC) -o $@ $(OBJS) $(OBJS_SUB) $(LIBS) $(INCLUDE)

SUB:
	@cd $(SUB_DIR)/;make all;

clean:
	@cd $(SUB_DIR)/;make clean;
	rm $(TARGETS) $(OBJS)
