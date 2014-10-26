// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"
#include "Map.h"
//#include "hunter.h"
#include "Places.h"

//Enable DEBUG mode (0 = FALSE, 1 = TRUE)
#define DEBUG 0 


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

    //Generate map of Europe
    Map europe;
    europe = newMap();
    assert(europe != NULL);

    //DEBUG
    if (DEBUG) {
        printf("europe is: %p\n", europe);
    }

    //Make a move in the shortest path towards Dracula
    shortestMove(gameState, possibleDestinations, numLocations, europe);

    //Make a move in the shortest path towards Dracula's last known location (within trail)
    lastKnownMove(gameState, possibleDestinations, numLocations, europe);

    
}
 
//Make a move at random from all possible moves   
LocationID randomMove(HunterView gameState, LocationID possibleDestinations[], int numLocations) 
{

    srand(time(NULL));
    int randomDestination = rand() % numLocations;

    //Register Play
    registerBestPlay(idToAbbrev(possibleDestinations[randomDestination]), "Random Move...");

    //DEBUG
    if (DEBUG) {
        printf("randomMove successful\n");
    }

    //Return chosen destination
    return possibleDestinations[randomDestination];
}


//Make a move if Dracula's current location is within the Hunter's possible moves    
LocationID singleMove(HunterView gameState, LocationID possibleDestinations[], int numLocations) 
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

    //DEBUG
    if (DEBUG) {
        printf("singleMove successful\n");
    }

    //Return if no match is found
    return UNKNOWN_LOCATION;
}

//Make a move that is the first step of the shortest path to dracula
LocationID shortestMove(HunterView gameState, LocationID possibleDestinations[], int numLocations, Map europe)
{
    //DEBUG
    if (DEBUG) {
        printf("shortestMove entered\n");
    }

    //Determine where Player is
    LocationID hunterLocation;
    hunterLocation = whereIs(gameState, whoAmI(gameState));

    //DEBUG
    if (DEBUG) {
        printf("hunterLocation is: %d\n", hunterLocation);
    }

    //Determine where Dracula is
    LocationID draculaLocation;
    draculaLocation = whereIs(gameState, PLAYER_DRACULA);

    //DEBUG
    if (DEBUG) {
        printf("draculaLocation is: %d\n", draculaLocation);
    }

    //Initialise path[] and trans[]
    LocationID path[NUM_MAP_LOCATIONS];
    TransportID trans[NUM_MAP_LOCATIONS];

    //DEBUG
    if (DEBUG) {
        printf("path[] and trans[] initialised\n");
    }

    //Return first step along the route to Dracula, or UNKNOWN_LOCATION if invalid
    if (hunterLocation >= MIN_MAP_LOCATION && hunterLocation <= MAX_MAP_LOCATION) {

        //Call shortestPath
        shortestPath(europe, hunterLocation, draculaLocation, path, trans);

        //DEBUG
        if (DEBUG) {
            printf("shortestPath called\n");
        }

        //Register result
        registerBestPlay(idToAbbrev(path[1]), "Shortest Move...");

        //DEBUG
        if (DEBUG) {
            printf("hunterLocation is: %d; draculaLocation is: %d\n", hunterLocation, draculaLocation);
            printf("path[0] is: %d; path[1] is: %d\n", path[0], path[1]);
            printf("shortestMove successful\n");            
        }

        //Return first step
        return path[1];

    } else {

        //Return invalid result
        return UNKNOWN_LOCATION;
    }
}    

//Move to Dracula's last known location within the trail
LocationID lastKnownMove(HunterView gameState, LocationID possibleDestinations[], int numLocations, Map europe)
{
    //Initialise path[] and trans[]
    LocationID path[NUM_MAP_LOCATIONS];
    TransportID trans[NUM_MAP_LOCATIONS];

    //DEBUG
    if (DEBUG) {
        printf("path[] and trans[] initialised\n");
    }


    //DEBUG
    if (DEBUG) {
        printf("lastKnownMove entered\n");
    }

    //Determine where Player is
    LocationID hunterLocation;
    hunterLocation = whereIs(gameState, whoAmI(gameState));

    //DEBUG
    if (DEBUG) {
        printf("hunterLocation is: %d\n", hunterLocation);
    }

    //Determine where Dracula was last known to be
    LocationID draculaLocation;
    LocationID draculaTrail[TRAIL_SIZE];
    giveMeTheTrail(gameState, PLAYER_DRACULA, draculaTrail);
    int i;
    for (i = 0; i < TRAIL_SIZE; i++) {
        draculaLocation = draculaTrail[i];
        if (hunterLocation >= MIN_MAP_LOCATION && hunterLocation <= MAX_MAP_LOCATION) {
            break;
        }
    }

    //DEBUG 
    if (DEBUG) {
        printf("draculaLocation is: %d\n", draculaLocation);
    }

    //Return first step along the route to Dracula, or UNKNOWN_LOCATION if invalid
    if (hunterLocation >= MIN_MAP_LOCATION && hunterLocation <= MAX_MAP_LOCATION) {

        //Call shortestPath
        shortestPath(europe, hunterLocation, draculaLocation, path, trans);

        //DEBUG
        if (DEBUG) {
            printf("shortestPath called\n");
        }

        //Register result
        registerBestPlay(idToAbbrev(path[1]), "Last Known Move...");

        //DEBUG
        if (DEBUG) {
            printf("hunterLocation is: %d; draculaLocation is: %d\n", hunterLocation, draculaLocation);
            printf("path[0] is: %d; path[1] is: %d\n", path[0], path[1]);
            printf("lastKnownMove successful\n");            
        }

        //Return first step
        return path[1];

    } else {

        //Return invalid result
        return UNKNOWN_LOCATION;
    }
}