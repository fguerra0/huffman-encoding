/*
 Fernando Guerra
 Project 5
 
 This program creates a binary tree made up of individual structures that holds
 inputted integers from the user. This tree goes into a separate program where
 a binary search tree is created from the binary tree and prints out the numbers
 using the inorder algorithm. Final output is the number in ascending order.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"

typedef struct tree TREE; // my name for the tree structures

struct tree
{
    int data;
    TREE *left;
    TREE *right;
    TREE *parent;
};

/*
 This function starts the new binary tree by creating its root and its left 
 and right subtrees. Runtime is O(1).
 */
TREE
*createTree(int data, TREE *left, TREE *right)
{
    TREE *top = malloc(sizeof(TREE)); // tree has been initially created
    assert(top != NULL); // make sure it exists
    
    top->data = data;
    setLeft(top, left); // calls set left to update its left subtree
    setRight(top, right); // updates its right subtree
    top->parent = NULL;
    
    return top; // returns pointer to the newly created binary tree
}

/*
 This function destroys the tree using a postorder algorithm to destroy each node
 from the bottom-up. Runtime is O(h) where h is the height of the binary tree.
 */
void
destroyTree(TREE *root)
{
    if(root == NULL) // base case
        return;
    
    destroyTree(root->left); // goes all the way to the left
    destroyTree(root->right); // goes all the way to the right
    
    free(root); // deletes going bottom-up
    
    return;
}

/*
 This function returns the data in each node of the tree this pointer is pointing
 at. Runtime is O(1).
 */
int
getData(TREE *root)
{
    assert(root != NULL); // makes sure it exists
    
    return root->data;
}

/*
 This function returns a pointer to the node's left child. Runtime is O(1).
 */
TREE
*getLeft(TREE *root)
{
    assert(root != NULL); // makes sure it exists
    
    return root->left;
}

/*
 This function returns a pointer to the node's right child. Runtime is O(1).
 */
TREE
*getRight(TREE *root)
{
    assert(root != NULL); // makes sure it exists
    
    return root->right;
}

/*
 This function returns a pointer to the node's parent. Runtime is O(1).
 */
TREE
*getParent(TREE *root)
{
    assert(root != NULL); // makes sure it exists
    
    return root->parent;
}

/*
 This function updates a node's left subtree by attaching it to the node's 
 left child pointer and deleting the child's pointers if there are any. Runtime
 is O(1).
 */
void
setLeft(TREE *root, TREE *left)
{
    assert(root != NULL); // makes sure it exists
    
    if(left != NULL) // if left is not NULL
    {
        if(left->parent != NULL) // if left has a parent
        {
            if(left->parent->right == left)
            {
                left->parent->right = NULL; // detaches from parent
            }
            else if(left->parent->left == left)
            {
                left->parent->left = NULL; // detaches from parent
            }
        }
        left->parent = root; // parent pointer updated
    }
    
    root->left = left; // node's left pointer updated
    
    return;
}

/*
 This function updates a node's right subtree by attaching it to the node's
 right child pointer and deleting the child's pointers if there are any. Runtime
 is O(1).
 */
void
setRight(TREE *root, TREE *right)
{
    assert(root != NULL); // makes sure it exists
    
    if(right != NULL) // if right is not NULL
    {
        if(right->parent != NULL) // if right has a parent
        {
            if(right->parent->right == right)
            {
                right->parent->right = NULL; // detaches from parent
            }
            else if(right->parent->left == right)
            {
                right->parent->left = NULL; // detaches from parent
            }
        }
        right->parent = root; // parent pointer updated
    }
    
    root->right = right; // node's right pointer updated
    
    return;
}
