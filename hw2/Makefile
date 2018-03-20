CC=gcc
CCFLAGS=-Wall -g -std=gnu90

%.o:%.c
	$(CC) $(CCFLAGS) -o$@ -c $<

%.tex:%.gp
	gnuplot $<
%.pdf:%.latex
	pdflatex $<

pblend:pblend.o
	$(CC) $(CCFLAGS) -o$@ $<

.PHONY: clean
clean:
	-rm -vf *.o pblend *.out *.err *.aux *.log *.html *.pdf *.tex

writeup.pdf:hw2.pdf

hw2.pdf:times.tex hw2.latex

