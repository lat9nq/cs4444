CC=gcc
CCFLAGS=-Wall -O3
LIBS=-lm

%.o:%.c
	$(CC) $(CCFLAGS) -o$@ -c $< $(LIBS)
%:%.o
	$(CC) $(CCFLAGS) -o$@ $< $(LIBS)

all:
	+make original
	+make optimized
	+make generate_input

optimized:optimized.o
original:original.o
generate_input:generate_input.o

.PHONY: clean
clean:
	-rm -vf original optimized generate_input *.o
	

