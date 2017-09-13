/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

void sort(int[], int);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    //Return error if n is not positive integer
    if(n < 1){
        printf("Error: must provide size of array as positive integer");
        return 2;
    }
    
    //Set bounds for search
    int low = 0;
    int high = n - 1;
    
    
    //Run binary search on sorted array
    int mid = 0;
    
    //End if length of array is less than 1
    while(high >= low) {
        mid = (high + low) / 2;
        if(value == values[mid]){
            return true;
        } else if (value < values[mid]){
            //If in lower half, run search again using lower half of array
            high = mid - 1;
        } else {
            //If in upper half, run search again using upper half of array
            low = mid + 1;
        }
    }
    
    //If the search doesn't return true, return false
    return false;
}


/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
    
    //Declare counting array with size of 65,536
    int count[65536] = {0};
    
    //Declare counter for target index
    int targetCount = 0;
    
    //Loop through values array and count up each number
    for(int i = 0; i < n; i++) {
        count[values[i]]++;
    }
    
    //Loop through count array and add numbers to target array
    for(int j = 0; j < 65536; j++) {
        for(int x = count[j]; x > 0; x--){
            values[targetCount] = j;
            targetCount++;
        }
    }
}
