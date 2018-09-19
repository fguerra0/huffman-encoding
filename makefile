CC	= gcc
CFLAGS	= -g -Wall

all:		treesort huffman

treesort:	treesort.o tree.o
		$(CC) -o treesort treesort.o tree.o

huffman:	huffman.o tree.o pack.o
		$(CC) -o huffman huffman.o tree.o pack.o

clean:;		rm -f *.o treesort huffman core core.*
