/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

bool check(const char *word);
bool load(const char *dictionary);
void trie(const char *word, struct Tnode *tnode);
struct Tnode *newNode(void);
unsigned int size(void);
bool unload(void);
void freeNodes(struct Tnode *troot);

struct Tnode *root;
FILE *dict;
char word[LENGTH + 1];

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    //open dictionary file
    dict = fopen(dictionary, "r");

    //initialize root node
    root = newNode();

    if(dict == NULL)
    {
        fprintf(stderr, "%s could not be opened\n", dictionary);
        return false;
    }

    // Declare word array and index variable


    //Generate trie
    while (fscanf(dict, "%s", word) != EOF)
    {
        //Call trie function
        trie(word, root);
    }

    return true;
}

//Input new word to trie
void trie(const char *word, struct Tnode *tnode)
{
    int index;
    int length = strlen(word);

    struct Tnode *keyNode = tnode;

    //Loop through characters
    for(int i = 0; i < length; i++)
    {
        //Determine index in tries array for letter, with apostrophe as 26
        if(isalpha(word[i]))
        {
            index = ((int) word[i] - (int) 'a');
        }
        else
        {
            index = 26;
        }

        //If node point is NULL, create new node
        if(!keyNode->children[index])
            keyNode->children[index] = newNode();

        //Move to next level
        keyNode = keyNode->children[index];
    }

    //Mark last node as word
    keyNode->is_word = true;
}

//Create new trie node
struct Tnode *newNode(void)
{
    //Initialize new node pointer
    Tnode *thisNode = NULL;

    //Allocate memory
    thisNode = (struct Tnode *)malloc(sizeof(struct Tnode));

    //Initialize all new children to NULL and set is_word to false
    if(thisNode)
    {
        thisNode->is_word = false;

        for(int i = 0; i < ALPHABET; i++)
            thisNode->children[i] = NULL;
    }

    return thisNode;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int length = strlen(word);
    int index;

    struct Tnode *tNode = root;

    for (int i = 0; i < length; i++)
    {
        //Determine index in tries array for letter, with apostrophe as 26
        if(isalpha(word[i]))
        {
            index = (int) tolower(word[i]) - (int) 'a';
        }
        else
        {
            index = 26;
        }

        if (!tNode->children[index])
            return false;

        tNode = tNode->children[index];
    }

    return (tNode != NULL && tNode->is_word);
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    int count = 0;

    //Return cursor to start of file
    fseek(dict, 0, SEEK_SET);

    //Generate trie
    while (fscanf(dict, "%s", word) != EOF)
        count++;


    return count;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{

    //free nodes
    freeNodes(root);

    // Close dictionary
    fclose(dict);


    return true;
}

void freeNodes(struct Tnode *troot)
{
    for(int i = 0; i < ALPHABET; i++)
    {
        if(troot->children[i] != NULL)
            freeNodes(troot->children[i]);
    }


    if(troot != NULL)
        free(troot);
}