// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"
#include <time.h>

//Function Prototypes
LocationID randomMove(HunterView gameState, LocationID* possibleDestinations, int numLocations);
LocationID singleMove(HunterView gameState, LocationID* possibleDestinations, int numLocations);
LocationID shortestMove(HunterView gameState, LocationID* possibleDestinations, int numLocations);

//"Main" Function
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

    //Make a move in the shortest path towards Dracula
    shortestMove(gameState, possibleDestinations, numLocations);


    
}
 
//Make a move at random from all possible moves   
LocationID randomMove(HunterView gameState, LocationID* possibleDestinations, int numLocations) 
{

    srand(time(NULL));
    int randomDestination = rand() % numLocations;

    //Register Play
    registerBestPlay(idToAbbrev(possibleDestinations[randomDestination]), "Random Move...");

    //Return chosen destination
    return possibleDestinations[randomDestination];
}


//Make a move if Dracula's current location is within the Hunter's possible moves    
LocationID singleMove(HunterView gameState, LocationID* possibleDestinations, int numLocations) 
{
    //Determine where Dracula is
    LocationID draculaLocation;
    draculaLocation = whereIs(gameState, PLAYER_DRACULA);

    //Loop through possibleDestinations to find a match for Dracula's location
    int i;
    for (i = 0; i < numLocations; i++) {
        if (draculaLocation == possibleDestinations[i]) {
            registerBestPlay(idToAbbrev(possibleDestinations[i]), "Single Move...");
            return possibleDestinations[i];
        }
    }

    //Return if no match is found
    return UNKNOWN_LOCATION;
}

//Make a move that is the first step of the shortest path to dracula
LocationID shortestMove(HunterView gameState, LocationID* possibleDestinations, int numLocations)
{
    //Determine where Dracula is
    LocationID draculaLocation;
    draculaLocation = whereIs(gameState, PLAYER_DRACULA);

    return draculaLocation; //To shut up gcc
}    