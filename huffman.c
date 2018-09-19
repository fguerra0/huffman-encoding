/*
 Fernando Guerra
 Project 5
 
 This program helps compress a file using Huffman coding algorithm and the pack function to create a Huffman
 tree that makes a bit encoding for each character in the file. The file will be compressed into a new file
 which can be decompressed using gunzip function.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h> // contains the isprint function for later
#include "tree.h"
#include "pack.h"

#define l(i) (2 * i + 1) // left child index in heap
#define r(i) (2 * i + 2) // right child index in heap
#define p(i) ((i - 1) / 2) // parent index in heap

typedef struct tree TREE;

TREE *heap[257];
TREE *leaves[257];
int numberOfTrees = 0; // the counter
int counts[257]; // character counter array

void occur(FILE *);
void insert(TREE *);
TREE *removeLow(TREE **);
void encoding(TREE *);

/*
 This function is the main function of this program; it runs the entire program, makes the tree and 
 encrypts the data in your compressed file.
 */
int
main(int argc, char *argv[])
{
    assert(argc == 3); // makes sure all three arguments are there
    
    int i;
    
    for(i = 0; i < 257; i++)
    {
        heap[i] = NULL;
        leaves[i] = NULL; // initializes empty arrays for both
    }
    
    FILE *fp;
    fp = fopen(argv[1], "r"); // file pointer created to file-to-be-compressed
    
    occur(fp);
    
    while(numberOfTrees > 1)
    {
        TREE *lowOne = removeLow(heap);
        TREE *lowTwo = removeLow(heap); // remove two lowest trees in the binary heap
        TREE *newTree;
        newTree = createTree((getData(lowOne) + getData(lowTwo)), lowOne, lowTwo); // combine the two lowest trees
        insert(newTree); // reinsert tree into heap until the huffman tree is the only tree left
    }
    
    for(i = 0; i < 256; i++)
    {
        int numb;
        
        if(counts[i] != 0) // if the character occurs in the file
        {
            numb = counts[i]; // number of occurrences in the file
        
            if(isprint(i) != 0) // checks if the character is printable or not
                printf("'%c': %d ", i, numb); // prints the character if it is printable
            else
                printf("%03o: %d ", i, numb); // prints the octal value of the character if it is not printable
            
            encoding(leaves[i]); // encodes the value of the character according to huffman tree and prints out the bits
            printf("\n"); // new line
        }
    }
    
    printf("EOF: 0 "); // end of file leaf gets printed out all by itself at the end
    encoding(leaves[256]);
    printf("\n");
    
    pack(argv[1], argv[2], leaves); // file is compressed using pack to create a .z file that can be decompressed using gunzip
    
    fclose(fp); // close the file pointer
    
    return 0; // end the program
}

/*
 This function counts the number of characters in the file that will be compressed and is pointed to by the file pointer
 currently open and inserts them into the leaves and then immediately put into the heap/priority queue.
 */
void
occur(FILE *fp)
{
    char c;
    int i;
    
    for(i = 0; i < 257; i++)
    {
        counts[i] = 0; // initialize empty counting array
    }
    
    while((c = fgetc(fp)) != EOF)
    {
        counts[(int)c]++; // counts every character's occurrence by the character
    }
    
    for(i = 0; i < 256; i++)
    {
        if(counts[i] > 0) // if the character is in the file a tree with its count number is created
        {
            leaves[i] = createTree(counts[i], NULL, NULL);
            insert(leaves[i]); // immediately insert the leaves of the characters into the heap
        }
        else
            leaves[i] = NULL; // if a character is not in the file set the leaf empty
    }
    
    leaves[256] = createTree(0, NULL, NULL); // end of file leaf is set to 0 and inserted to the heap
    insert(leaves[256]);
    
    return;
}

/*
 This function is the function that removes the lowest valued tree from the heap and returns the pointer 
 to that array back to the loop while the number of trees is still grater than 1.
 */
TREE
*removeLow(TREE *array[])
{
    TREE *lowest; // tree pointer declared
    TREE *temp; // temporary tree pointer declared
    
    int i;
    i = 0;
    
    lowest = array[0]; // the smallest value is at the front of the heap array
    array[0] = array[numberOfTrees - 1]; // swap with the last item in the array
    array[numberOfTrees - 1] = NULL; // set the last item in the array to empty pointer
    
    while(array[l(i)] != NULL) // keep checking if the item has a left child; will end when it does not have a left child anymore which means it's the end of the heap
    {
        if(array[r(i)] != NULL) // check if the item also has a right child
        {
            if(getData(array[i]) > getData(array[l(i)]) || getData(array[i]) > getData(array[r(i)])) // if the item is greater than at least one of its children
            {
                if(getData(array[l(i)]) < getData(array[r(i)])) // if the left is smaller than the right child it will be the smaller of the item's children
                {
                    temp = array[i];
                    array[i] = array[l(i)]; // swap the item with its left child
                    array[l(i)] = temp;
                    i++;
                }
                else
                {
                    temp = array[i];
                    array[i] = array[r(i)]; // otherwise swap the item with its right child
                    array[r(i)] = temp;
                    i++;
                }
            }
            else
            {
                break; // if the item is already smaller than both of its children then do not do anything
            }
        }
        else // if the item only has a left child
        {
            if(getData(array[i]) > getData(array[l(i)])) // if the item is bigger than its left child then swap them
            {
                temp = array[i];
                array[i] = array[l(i)];
                array[l(i)] = temp;
                i++;
            }
            else
            {
                break; // if the item is smaller than its only child then do not do anything
            }
        }
    }
    numberOfTrees--; // with the lowest value gone, decrease the number of trees in the heap
    
    return lowest; // lowest tree's pointer is returned
}

/*
 This is the function that inserts trees into the priority queue/heap in correct heap order. By the end, the last tree inserted
 back into the heap will be the final huffman tree.
 */
void
insert(TREE *combinedTree)
{
    assert(combinedTree != NULL); // make sure the tree is not empty
    
    TREE *temp;
    int holder = numberOfTrees; // total number of trees
    heap[holder] = combinedTree;
    
    while(holder > 0)
    {
        if(getData(heap[holder]) < getData(heap[p(holder)])) // checks to see if the child is smaller than its parent value
        {
            temp = heap[holder];
            heap[holder] = heap[p(holder)]; // keeps swapping the place of the tree in the heap while its parent's value is greater than its own value
            heap[p(holder)] = temp;
            holder = p(holder); // index has been updated
        }
        else
            break;
    }
    
    numberOfTrees++; // increase the tree counter as the tree is goes back into the heap
    
    return;
}

/*
 This function goes through the huffman tree by traveling up from the leaf to the root and printing out the bit encoding bit by bit using recursion depending on if
 it goes down either a left or right branch until it reaches the leaf again. This will create and print out a unique bit code for that specific character.
 */
void
encoding(TREE *np)
{
    if(getParent(np) == NULL) // basic case i.e. it has reached the root of the tree
        return;
    
    encoding(getParent(np)); // keeps calling this function using the parent of that leaf until something is returned
    
    if(np == getLeft(getParent(np))) // if the leaf is to the left of its parent, a 0 is printed out
        printf("0");
    else
        printf("1"); // if the leaf is to the right of its parent, a 1 is printed out
    
    return;
}
