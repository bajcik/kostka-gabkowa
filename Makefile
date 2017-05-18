all: kostka-gabkowa test

kostka-gabkowa: kostka-gabkowa.c
	gcc -ggdb -Wall -o kostka-gabkowa $<

test: kostka-gabkowa.c
	gcc -ggdb -Wall -o test -DTEST $<
	

