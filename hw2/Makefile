CC=gcc
CCFLAGS=-Wall -g -std=gnu90

%.o:%.c
	$(CC) $(CCFLAGS) -o$@ -c $<

pblend:pblend.o
	$(CC) $(CCFLAGS) -o$@ $<

.PHONY: clean
clean:
	-rm -vf *.o pblend *.out *.err

