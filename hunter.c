// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"
#include <time.h>

LocationID randomMove(HunterView gameState, *LocationID possibleDestinations, int numLocations);
LocationID singleMove(HunterView gameState, *LocationID possibleDestinations, int numLocations);


void decideHunterMove(HunterView gameState)
{

    //Determine all possible moves
    int numLocations;
    LocationID *possibleDestinations; 
    possibleDestinations = whereCanIgo(gameState, &numLocations, 1, 1, 1);

    //Make a move at random
    randomMove(gameState, possibleDestinations, numLocations);

    //Make a single-step move
    singleMove(gameState, possibleDestinations, numLocations);


    
}
 
//Make a move at random from all possible moves   
LocationID randomMove(HunterView gameState, *LocationID possibleDestinations, int numLocations) 
{

    srand(time(NULL));
    int randomDestination = rand() % numLocations;

    //Register Play
    registerBestPlay(idToAbbrev(possibleDestinations[randomDestination]), "Random Move...");

    return possibleDestinations[randomDestination];
}


//Make a move if Dracula's current location is within the Hunter's possible moves    
LocationID singleMove(HunterView gameState, *LocationID possibleDestinations, int numLocations) 
{
    //Determine where Dracula is
    LocationID draculaLocation;
    draculaLocation = whereIs(gameState, PLAYER_DRACULA);

    int i;
    for (i = 0; i < numLocations; i++) {
        if (draculaLocation = possibleDestinations[i]) {
            registerBestPlay(idToAbbrev(possibleDestinations[i]), "Single Move...");
            return possibleDestinations[i];
        }
    }

    return UNKNOWN_LOCATION;
}