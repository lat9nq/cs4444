# Makefile
# 
# Louis Thomas
# Prof. Andrew Grimshaw
# Assignment 1
# 30 January 2018

# ~~~~~~ COMPILERS AND FLAGS ~~~~~~

CC=gcc
CCFLAGS=-Wall -Ofast -fno-stack-protector
LIBS=-lm

HIGHLIGHT=source-highlight -n

TEX=pdflatex

# ~~~~~~ SUFFIX RULES ~~~~~~

%.o:%.c
	$(CC) $(CCFLAGS) -o$@ -c $< $(LIBS)
%:%.o
	$(CC) $(CCFLAGS) -o$@ $< $(LIBS)

%.html:%.c
	$(HIGHLIGHT) -i $< > $@
%.pdf:%.html
	wkhtmltopdf -B 1in -L 1in -R 1in -T 1in --minimum-font-size 18 -s letter $< $@
%.pdf:%.tex
	$(TEX) $<

# ~~~~~~ EXPLICIT RULES ~~~~~~

all:
	+make original
	+make optimized
	+make generate_input

optimized:optimized.o
original:original.o
generate_input:generate_input.o

# Produces the input used in the writeup. May not be exact from machine-to-machine.
a.in:generate_input
	./generate_input 10000 10 > a.in

writeup.pdf:analysis.pdf optimized.pdf
	pdfunite analysis.pdf optimized.pdf writeup.pdf

.PHONY: clean tar
clean:
	-rm -vf original optimized generate_input *.o *.aux *.log *.gmon analysis.pdf optimized.pdf writeup.pdf *.html

TAR_REQ=optimized.c writeup.pdf generate_input.c original.c Makefile analysis.tex
tar:$(TAR_REQ)
	tar -c $(TAR_REQ) > 180130_assignment_1.tar


