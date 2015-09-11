CC      =   gcc
CFLAGS  += -g -Wall -D_x86
TARGET  = ./process
SOURCES = $(wildcard *.c)
OBJS    = $(patsubst %.c,%.o,$(SOURCES))

MYLIBS   = -I.

all:process

%c.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -lpthread

	chmod a+x $(TARGET)

.PHONY: clean rf
clean:
	rm -rf *.o $(TARGET)

rf:all


