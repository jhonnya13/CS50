#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();


        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO
    // Look for candidate name
    int i = 0;
    while (i < candidate_count)
    {
        // If found
        if (strcasecmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    i++;
    }
    // If not found
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int z = 0; z < candidate_count; z++)
    {
        printf("Candidate_count: %i" , candidate_count);
        printf("1_Name: %s, vote: %i.\n" , candidates[z].name, candidates[z].votes);
    }

    for (int i = 0; i < voter_count; i++)
    {
        int current_index = preferences[i][0];
        if (candidates[current_index].eliminated == false)
        {
            candidates[current_index].votes += 1;
            //printf("Name: %s, vote: %i.\n" , candidates[current_index].name, candidates[current_index].votes);
            break;
        }
        if (candidates[current_index].eliminated == true)
        {
            i++;
        }
    }
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // Go thro all canditates
    int result = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if any == half all voutes
        if (candidates[i].votes > voter_count/2)
        {
            // If yes - print name an return true
            printf("%s\n", candidates[i].name);
            result = 1;
        }
        result = 0;
    }
    return result;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int minimum = MAX_VOTERS;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false)
        {
            // Compare value with minimum
            if (candidates[i].votes < minimum)
            {
                // Overwrite minimum
                minimum = candidates[i].votes;
            }
        }
    }
    return minimum;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // Go thro every candidate
    int i = 0;
    int result = 0;
    while ( i < candidate_count)
    {
        // Check if candidate eliminated
        if (candidates[i].eliminated == false)
        {
            if (candidates[i].votes == min)
            {
                result = 1;
            }
            result = 0;
        i = i + 1;
        }
    }
    if (result == 1)
    {
        return true;
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    // Go thro all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if candidates vote == minimum and eliminate
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}