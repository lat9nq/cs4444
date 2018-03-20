CC=gcc
CCFLAGS=-Wall -Ofast -std=gnu90
LIBS=-lm -lpthread

%.o:%.c
	$(CC) $(CCFLAGS) -o$@ -c $< $(LIBS)

heated_plate:heated_plate.o
	$(CC) $(CCFLAGS) -o$@ $< $(LIBS)

.PHONY:clean
clean:
	-rm -vf heated_plate *.o *.ppm
