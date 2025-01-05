// Andrea Marquez Tavera
// COP3502C Section 3
// Engine Shutdown

/*------------------------------------------------------------
OBJECTIVE

Give practice with brute force and recursion in C.
------------------------------------------------------------*/

#include <stdio.h>

#define GARBAGE -1// used to reset/initialize current permutations array
#define ACTIONS_MAXIMUM 11

// global variables to make code easier to read
int bestPerm[ACTIONS_MAXIMUM];
int bestTemp = 100000000;// ridiculously high number to compare/replace with current temp

// user defined functions
void permute(int * used, int * perm, int index, int actions, int grid[actions][actions]);// finds best permutation

// The main function
int main() {
    int actions;// stores amount of actions from user
    int valid = 0;/// trigger to make sure actions is within bounds

    while(valid != 1)
    {   
        scanf("%d", &actions);
        if(actions >= 1 && actions<= 11)
        {
            valid = 1;// if valid then continue on
        }
    }

    // scan second line input and then forget about it... lol
    int baseTemp[actions];
    for(int i = 0; i < actions; i++)
    {
        scanf("%d", &baseTemp[i]);
    }

    // scan 2D array that will hold each temperature that's affected by position in the permutation
    int affectedTempArr[actions][actions];
    for(int i = 0; i < actions; i++)
    {
        for(int j = 0; j < actions; j++)
        {
            scanf("%d", &affectedTempArr[i][j]);
        }
    }

    int used[actions]; // 0 or 1 representing which values have been used
    int perm[actions]; // The array to hold the values in the permutation
    
    // Initialize the array
    for (int i = 0; i < actions; i++) 
    {
        used[i] = 0;
        perm[i] = GARBAGE;
    }
    
    // recurse through all the permutations and find best one
    permute(used, perm, 0, actions, affectedTempArr);

    // print/output best permutation
    for (int i = 0; i < actions; i++)
        printf("%d ", bestPerm[i] + 1);
    printf("\n");
    
    // Exit
    return 0;
}

void permute(int * used, int * perm, int index, int actions, int grid[actions][actions])
{
    // Check if the index is invalid
    if (actions == index) 
    {   
        int curTemp = 0;// used to compare against current value of bestTemp

        for(int i = 0; i < actions; i++)// add each temperature based on the order of permutation, the original 2D array, start, and end
        {
            for(int j = i + 1; j < actions; j++)
            {
                curTemp += grid[perm[j]][perm[i]];
            }
        }
        // check to see which temperature is better, this current one or the value globally saved in bestTemp
        if(curTemp < bestTemp)// if this temperature is better than bestTemp, replace bestTemp and bestPerm
        {
            bestTemp = curTemp;
            for(int i = 0; i < actions; i++)
            {
                bestPerm[i] = perm[i];
            }
        }
        return; // prevent calling recursively
    }
    
    for (int curValue = 0; curValue < actions; curValue++)
    {
        // Check if the value is not allowed
        if (used[curValue]) continue; // skip
        
        // Use the value
        used[curValue] = 1;
        perm[index] = curValue;
        // Recurse
        permute(used, perm, index + 1, actions, grid);
    
        // Unuse the value
        used[curValue] = 0;
        perm[index] = GARBAGE;
    } 
}