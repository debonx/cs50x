#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <search.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    int stored = 0;
    int i = 0;
    while (i < candidate_count)
    {
        if (0 == strcmp(candidates[i], name))
        {
            ranks[rank] = i;
            stored = 1;
        }
        i++;
    }
    return stored;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int i = 0;
    int j = 0;
    while (i < candidate_count)
    {
        j = i + 1;
        while (j < candidate_count)
        {
            preferences[ranks[i]][ranks[j]]++;
            j++;
        }
        i++;
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int i = 0;
    int j = 0;
    while (i < candidate_count)
    {
        j = i + 1;
        while (j < candidate_count)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
            j++;
        }
        i++;
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int i = 0;
    int j = 0;
    pair temp;

    while (i < pair_count)
    {
        j = i + 1;
        while (j < pair_count)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[j].winner][pairs[j].loser])
            {
                temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
            j++;
        }
        i++;
    }
    return;
}

bool can_lock(int winner, int loser)
{
    while (winner)
    {
        bool cycle = false;
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[i][winner])
            {
                cycle = true;
                winner = i;
            }
        }
        if (!cycle)
        {
            break;
        }
        if (winner == loser)
        {
            return false;
        }
    }
    return true;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int i = 0;
    while (i < pair_count)
    {
        if (can_lock(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
        i++;
    }
}

// Print the winner of the election
void print_winner(void)
{
    int i = 0;
    while (i < candidate_count)
    {
        bool winner = true;
        int j = 0;
        while (j < candidate_count)
        {
            if (locked[j][i])
            {
                winner = false;
                break;
            }
            j++;
        }
        if (winner)
        {
            printf("%s\n", candidates[i]);
            return;
        }
        i++;
    }
    return;
}

