// hunter.h
// Interface to your "Fury of Dracula" hunter AI
// By: TeamDracula
// Date: 1 January 2012
// Version: 1.0
#include "Map.h"

//"Main" Function
void decideHunterMove(HunterView gameState);

// New Functions

//Make a move at random from all possible moves   
//If a valid move if found, function calls registerBestPlay()
//Returns the locationID correpsonding to the Location Initials registered
LocationID randomMove(HunterView gameState, LocationID possibleDestinations[], int numLocations);

//Make a move if Dracula's current location is within the Hunter's possible moves 
//If a valid move if found, function calls registerBestPlay()
//Returns the locationID correpsonding to the Location Initials registered
LocationID singleMove(HunterView gameState, LocationID possibleDestinations[], int numLocations);

//Make a move that is the first step of the shortest path to dracula
//If a valid move if found, function calls registerBestPlay()
//Returns the locationID correpsonding to the Location Initials registered
LocationID shortestMove(HunterView gameState, LocationID possibleDestinations[], int numLocations, Map europe);

//Move to Dracula's last known location within the trail
//If a valid move if found, function calls registerBestPlay()
//Returns the locationID correpsonding to the Location Initials registered
LocationID lastKnownMove(HunterView gameState, LocationID possibleDestinations[], int numLocations, Map europe);
