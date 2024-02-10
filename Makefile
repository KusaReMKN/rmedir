TARGET=	rmedir
OBJS=	rmedir.o

SHELL=	/bin/sh
CC=	cc
CFLAGS=	-O2 -Wall
RM=	rm -f

.PHONY:	all clean

all:	$(TARGET)

$(TARGET):	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
