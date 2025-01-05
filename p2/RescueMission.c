// Andrea Marquez Tavera
// COP3502C Section 3
// Rescue Mission

/*------------------------------------------------------------
OBJECTIVE

Give practice with binary search in C.

PROBLEM

Write a program that when given the maximum starting speed of the ship and the maximum x coordinate, can ask for information at specific x-coordinates at most 23 hrsSinceSignals, and can at any point after a query can report the current location of the ship.

------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int binarySearch(int location, int speed, int possibleBoatsArr[location + 1][speed + 1], int flagsBoatArr[location + 1][speed + 1]);
void sortNum(int location, int speed, int counter, int allNumsArr[(location + 1) * (speed + 1)]);
void checkInput(int location, int speed, char inputArr[7 + 1], int possibleBoatsArr[location + 1][speed + 1], int outputPos, int * possibleBoats, int flagsBoatArr[location + 1][speed + 1]);

int main() {
    int maxSpeed;// will take max speed
    int maxLocation;// will take max location
    int acceptable = 0;// used as a trigger for while loop
    int hrsSinceSignal = 0;// will keep track of hours since the signal

    while(acceptable != 1)// checks if speed and location are in range
    {
        scanf("%d %d", &maxSpeed, &maxLocation);
        if((maxSpeed >= 1) && (maxLocation <= 100))
        {
            acceptable = 1;// if speed and location are in range then continue
        }
    }

    // Create two 2D array:
    // 1. keeps track of flags 
    //      (1 is possible, 0 is not possible)
    // 2. keeps track of boat position values

    int flaggedBoats[maxLocation + 1][maxSpeed + 1];// creates 2D grid to flag if possible or not
    for(int i = 0; i <= maxLocation; i++)
    {
        for(int j = 0; j<= maxSpeed; j++)
        {
            flaggedBoats[i][j] = 1;
        }
    }

    int possibleBoats[maxLocation + 1][maxSpeed + 1];// creates 2D grid to store boat positions
    int totalBoats = (maxLocation + 1) * (maxSpeed + 1);// total boat combinations, + 1 to account for 0 in 2D array
    for(int i = 0; i <= maxLocation; i++)// loops through and initializes all possible boat combinations
    {
        for(int j = 0; j <= maxSpeed; j++)
        {
            possibleBoats[i][j] = j + i;// values are not accounted to hoursSinceSignal initially since it's set to 0... so the math wouldn't math
        }
    }

    // queries
    char found[7 + 1];// longest possible response is 7 characters
    while(hrsSinceSignal < 24)// max amount of photos taken is 23
    {   
        int guessOutput = binarySearch(maxLocation, maxSpeed, possibleBoats, flaggedBoats);

        // if there's only one boat left possible, rescue that boat's current position
        if(totalBoats == 1)
        {
            printf("! %d\n", guessOutput);
            fflush(stdout);
            return 0;
        }

        // otherwise guess boat from binary search
        printf("? %d\n", guessOutput);
        fflush(stdout);

        scanf(" ");// takes enter key from first user input
        fgets(found, 7+1, stdin);
        
        // checks if input is wake, no wake, or boat
        checkInput(maxLocation, maxSpeed, found, possibleBoats, guessOutput, &totalBoats, flaggedBoats);
        strcpy(found,  " ");// nulls string for new input

        hrsSinceSignal+= 1;// update amount of pics taken

    }
    return 0;// did not guess within 24 hours
}

int binarySearch(int location, int speed, int possibleBoatsArr[location + 1][speed + 1], int flagsBoatArr[location + 1][speed + 1])
{
    int allNumsArr[(location + 1)*(speed + 1)];// will store a 1D array of all possible boats
    int counter = 0;// keeps track of new array index

    // save all boats in 1D array
    for(int i = 0; i <= location; i++)// loops through every boat
    {
        for(int j = 0; j <= speed; j++)
        {
            if(flagsBoatArr[i][j] == 1)// as long as boat is not disabled
            {   
                allNumsArr[counter] = possibleBoatsArr[i][j];// save boat possibility into new array
                counter++;// increase index of array to add to the next available slot
            }
        }
    }

    // calls function to sort 1D array in numerical order
    sortNum(location, speed, counter, allNumsArr);

    // if only one boat left guess that boat's current position
    if(counter == 1)
    {
        return allNumsArr[0];
    }
    // otherwise return midpoint of array
    return allNumsArr[(counter / 2) - 1];// minus one to take acount of index 0
}

void sortNum(int location, int speed, int counter, int allNumsArr[(location + 1) * (speed + 1)])
{
    int temp;// will store temp variable to organize array

    // sorts boats in array in increasing numerical order
    for(int i = 0; i < counter; i++)// loop through all possible boats
    {
        for(int j = i; j < counter; j++)// loop again to compare every speed to every location combo
        {
            // if first boat is greater than the next then reorganize
            if(allNumsArr[i] > allNumsArr[j])
            {
                temp = allNumsArr[i];
                allNumsArr[i] = allNumsArr[j];
                allNumsArr[j] = temp;
            }
        }
    }
}

void checkInput(int location, int speed, char inputArr[7 + 1], int possibleBoatsArr[location + 1][speed + 1], int outputPos, int * possibleBoats, int flagsBoatArr[location + 1][speed + 1])
{
    // if input is wake (originally used strcmp but I'm not sure why that didn't work)
    if(inputArr[0] == 'W')
    {
        for(int i = 0; i <= location; i++)// loop through boat possibilities and deactivate anything before said x-pos
        {
            for(int j = 0; j <= speed; j++)
            {
                if((possibleBoatsArr[i][j] <= outputPos) && (flagsBoatArr[i][j] == 1))// if the boat is before the x-pos guessed AND it wasn't already disabled
                {
                    flagsBoatArr[i][j] = 0;// disable boat
                    *possibleBoats -= 1;// lower total number of possible boats active
                    // this is why we don't want to count if the boat was already disabled since it was already accounted for
                }
                else if(flagsBoatArr[i][j] == 1)// otherwise if boat was not already disabled
                {
                    possibleBoatsArr[i][j] = possibleBoatsArr[i][j] + j;// update that boat to its value plus speed because it has moved
                }
            }
        }
    }
    else if(inputArr[0] == 'N')// otherwise if response is No Wake, meaning boat has not passed that x-pos
    {
        for(int i = 0; i <= location; i++)// loop through all possible boats
        {
            for(int j = 0; j <= speed; j++)
            {
                if((possibleBoatsArr[i][j] >= outputPos) && (flagsBoatArr[i][j] == 1))// disable any boat after x-pos since those are not possible, as long as it wasn't already disabled
                {
                    flagsBoatArr[i][j] = 0;// disable boat
                    *possibleBoats -= 1;// lower total number of possible boats active, same logic as above
                }
                else if(flagsBoatArr[i][j] == 1)
                {
                    possibleBoatsArr[i][j] = possibleBoatsArr[i][j] + j;// same as above
                }
            }
        }
    }
    else if(inputArr[0] == 'B')// otherwise if the response is Boat!
    {
        for(int i = 0; i <= location; i++)// loop through each possible boat
        {
            for(int j = 0; j <= speed; j++)
            {
                if((possibleBoatsArr[i][j] != outputPos) && (flagsBoatArr[i][j] == 1))// disable boat if current boat is not the x-pos guessed and as long as it wasn't already disabled
                {
                    flagsBoatArr[i][j] = 0;// disable boat
                    *possibleBoats -= 1;// same logic as above
                }
                else if(flagsBoatArr[i][j] == 1)
                {
                    possibleBoatsArr[i][j] = possibleBoatsArr[i][j] + j;// same logic as above
                }
            }
        }
    }
}