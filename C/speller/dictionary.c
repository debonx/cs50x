
// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// Sum of all lowercase letters
const unsigned int N = 2616;

// Hash table
node *table[N];

// word counter
unsigned int countwords = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // return true if length == 1
    int wlength = strlen(word);
    if (wlength == 1)
    {
        return true;
    }

    char temp[wlength + 1];
    int i = 0;
    while (i < wlength)
    {
        temp[i] = tolower(word[i]);
        i++;
    }
    // close the word
    temp[wlength] = '\0';

    // get the node
    node *find = table[hash(temp)];

    while (find != NULL)
    {
        if (strcasecmp(word, find->word) == 0)
        {
            return true;
        }

        // search next
        find = find->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // sum of all lowercase char + the highest / 2
    unsigned int hash = (2616 * 97) / 2;
    int sumChar = 0;

    // summing characters
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        sumChar += word[i];
    }

    // sum of first / last
    int firstLast = word[0] + word[strlen(word) - 1];

    //subtracting the sum of first / last character and fit the table size
    return (hash - firstLast) % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open file
    FILE *file = fopen(dictionary, "r");

    // null character
    char temp[LENGTH + 1];

    /// reading words until the end
    while (fscanf(file, "%s", temp) != EOF)
    {
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            // rollback and bail
            unload();
            return false;
        }

        // storying the word
        strcpy(n->word, temp);
        // index / hashed word in the table
        int i = hash(n->word);

        // To the start of the list to not loose it
        n->next = table[i];

        // Store the node in relation to its hash
        table[i] = n;
        countwords++;
    }
    /// close file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return countwords;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // iterate through hashtable
    int i = 0;
    while (i < N)
    {
        // start of the list
        node *n = table[i];

        // go through it
        while (n != NULL)
        {
            node *temp = n;
            n = n->next;
            free(temp);
        }

        free(n);
        i++;
    }
    return true;
}