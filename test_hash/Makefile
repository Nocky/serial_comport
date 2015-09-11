CC      =   gcc
CFLAGS  += -g -Wall
TARGET  = ./test_hash
SOURCES = $(wildcard *.c)
OBJS    = $(patsubst %.c,%.o,$(SOURCES))

MYLIBS   = -I.

all:test_hash

%c.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -lpthread

	chmod a+x $(TARGET)

.PHONY : clean rf
clean:
	rm -rf *.o $(TARGET)

rf:all


