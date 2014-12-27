
CC = gcc

LDLIBS=`pkg-config --libs gtk+-2.0`
CFLAGS=-Wall -g `pkg-config --cflags gtk+-2.0`


OBJS = osdtool.o

osdtool.o: osdtool.c
	$(CC) $(CFLAGS) -c osdtool.c -o osdtool.o

osdtool: $(OBJS)
	$(CC) osdtool.o -o osdtool $(LDLIBS)

clean:
	rm -f *.o *~ osdtool

