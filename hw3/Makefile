CC=gcc
CCFLAGS=-Wall -Ofast -std=gnu90
LIBS=-lm -lpthread

%.o:%.c
	$(CC) $(CCFLAGS) -o$@ -c $< $(LIBS)

HEATED_OFILES=thread_work.o heated_plate.o
heated_plate:$(HEATED_OFILES)
	$(CC) $(CCFLAGS) -o$@ $(HEATED_OFILES) $(LIBS)

.PHONY:clean
clean:
	-rm -vf heated_plate *.o *.ppm

heated_plate.o:heated_plate.c thread_work.h
thread_work.o:thread_work.c thread_work.h
