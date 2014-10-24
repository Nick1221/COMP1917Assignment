// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"
#include <time.h>

LocationID randomMove(HunterView gameState);

void decideHunterMove(HunterView gameState)
{
    // TODO ...
    // Replace the line below by something better
    //registerBestPlay("GE","I'm on holiday in Geneva");

    // Original Code below

    //Determine character abbreviation
    //PlayerID characterID = whoAmI(gameState);
    
    //Make stupid move
    LocationID destinationID = randomMove(gameState);

    //printf("destinationID is: %d\n", destinationID);
    registerBestPlay(idToAbbrev(destinationID), "");
    
}
    
LocationID randomMove(HunterView gameState) 
{
    int numLocations;
    //LocationID possibleDestinations[NUM_MAP_LOCATIONS]; 
    LocationID *possibleDestinations; 
    possibleDestinations = whereCanIgo(gameState, &numLocations, 1, 1, 1);

    srand(time(NULL));
    int randomDestination = rand() % numLocations;

    return possibleDestinations[randomDestination];
}
    