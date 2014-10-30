// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"
#include "Map.h"
#include "Places.h"

//Enable DEBUG mode (0 = FALSE, 1 = TRUE)
#define DEBUG 0

//Set Health lower limit
#define HEALTH_CRITICAL 2

//Set starting positions
#define POSITION_0 MUNICH
#define POSITION_1 MADRID
#define POSITION_2 MANCHESTER
#define POSITION_3 CASTLE_DRACULA

// New Functions

//Make a pre-determined move for the first round
//Makes the move as defined via #define at the start of file
//Returns the locationID corresponding to the Location Initials registered
LocationID firstMove(HunterView gameState);

//Make a move at random from all possible moves   
//If a valid move if found, function calls registerBestPlay()
//Returns the locationID corresponding to the Location Initials registered
LocationID randomMove(HunterView gameState, LocationID possibleDestinations[], int *numLocations);

//Make a move if Dracula's current location is within the Hunter's possible moves 
//If a valid move if found, function calls registerBestPlay()
//Returns the locationID corresponding to the Location Initials registered
LocationID singleMove(HunterView gameState, LocationID possibleDestinations[], int *numLocations);

//Make a move that is the first step of the shortest path to dracula
//If a valid move if found, function calls registerBestPlay()
//Returns the locationID corresponding to the Location Initials registered
LocationID shortestMove(HunterView gameState, LocationID possibleDestinations[], int *numLocations, Map europe);

//Move to Dracula's last known location within the trail
//If a valid move if found, function calls registerBestPlay()
//Returns the locationID corresponding to the Location Initials registered
LocationID lastKnownMove(HunterView gameState, LocationID possibleDestinations[], int *numLocations, Map europe);

//Stay in the current location if health is low
//If hunter's health is below HEALTH_CRITICAL, then they will rest and regain 3 health
//Returns the locationID corresponding to the Location Initials registered
LocationID healthCriticalMove(HunterView gameState);

//Stay in the current location if research would be beneficial
//If Dracula's past location is unknown, and the trail is full then conduct research
//Returns the locationID corresponding to the Location Initials registered
LocationID researchMove(HunterView gameState);

//Find the shortest route to Castle Dracula
//Intended for use by only one hunter
//Returns the locationID corresponding to the Location Initials registered
LocationID directCastleMove(HunterView gameState, LocationID possibleDestinations[], int *numLocations, Map europe);







//"Main" Function
void decideHunterMove(HunterView gameState)
{
    //DEBUG
    if (DEBUG) {
        printf("decideHunterMove entered\n");
    }

    //DEBUG
    if (DEBUG) {
        Round currentRound = giveMeTheRound(gameState);
        printf("Round is: %d\n", currentRound);
    }

    //Special move for the first round
    if (giveMeTheRound(gameState) == 0) {
        firstMove(gameState);
        return;
    }

    //Bypass the fact that Gameview.c isn't properly accounting for railMod = 0
    //https://www.openlearning.com/courses/COMP1927-14s2/FuryOfDracula/Hunt?inCohort=courses/COMP1927-14s2/Cohorts/ClassOf2014#comment-5451be3cf860612b38fbb6a1

    //Determine whether rail is valid or not
    int railMod;
    int railModSum = whoAmI(gameState) + giveMeTheRound(gameState);
    railMod = railModSum % 4;
    //Simplify railMod to be 0 or 1
    if (railMod > 0) {
        railMod = 1;
    }

    //Determine all possible moves
    int *numLocations = malloc(sizeof(int));
    LocationID *possibleDestinations = whereCanIgo(gameState, &numLocations[0], 1, railMod, 1); 

    //DEBUG
    if (DEBUG) {
        printf("numLocations is: %d\n", numLocations[0]);
        int debugCounter;
        for (debugCounter = 0; debugCounter < numLocations[0]; debugCounter++) {
            printf("possibleDestinations[%d] is: %d\n", debugCounter, possibleDestinations[debugCounter]);
        }
    }

    //Determine where Player is
    LocationID hunterLocation;
    hunterLocation = whereIs(gameState, whoAmI(gameState));

    //DEBUG
    if (DEBUG) {
        printf("hunterLocation is: %d\n", hunterLocation);
    }

    //If player 3, take the shortest route to Castle Dracula and stay there
    if (whoAmI(gameState) == PLAYER_MINA_HARKER) {

        //Generate map of Europe
        Map europe;
        europe = newMap();
        assert(europe != NULL);

        //Execute move
        directCastleMove(gameState, possibleDestinations, numLocations, europe);

        //Abort the AI
        return;
    }

    //Make a move at random
    randomMove(gameState, possibleDestinations, numLocations);

    //Make a single-step move
    singleMove(gameState, possibleDestinations, numLocations);

    //Stay in the current location if health is low
    if (healthCriticalMove(gameState) == hunterLocation) {
        return;
    }

    //Stay in the current location if research would be beneficial
    if (researchMove(gameState) == hunterLocation) {
        return;
    }

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

    //DEBUG
    if (DEBUG) {
        printf("decideHunterMove exited\n");
    }

}
 
//Make a pre-determined move for the first round
LocationID firstMove(HunterView gameState) {

    //Determine hunterID
    PlayerID hunter = whoAmI(gameState);

    //Register appropriate best move
    if (hunter == 0) {
        registerBestPlay(idToAbbrev(POSITION_0), "First Move...");
        return POSITION_0;
    } else if (hunter == 1) {
        registerBestPlay(idToAbbrev(POSITION_1), "First Move...");
        return POSITION_1;
    } else if (hunter == 2) {
        registerBestPlay(idToAbbrev(POSITION_2), "First Move...");
        return POSITION_2;
    } else if (hunter == 3) {
        registerBestPlay(idToAbbrev(POSITION_3), "First Move...");
        return POSITION_3;
    }

    return UNKNOWN_LOCATION;

}

//Make a move at random from all possible moves   
LocationID randomMove(HunterView gameState, LocationID possibleDestinations[], int *numLocations) 
{
    //DEBUG
    if (DEBUG) {
        printf("randomMove entered\n");
    }

    //Generate random number
    srand(time(NULL));
    int randomDestination = rand() % numLocations[0];

    //DEBUG
    if (DEBUG) {
        printf("randomDestination is: %d\n", randomDestination);
    }

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
LocationID singleMove(HunterView gameState, LocationID possibleDestinations[], int *numLocations) 
{
    //DEBUG
    if (DEBUG) {
        printf("singleMove entered\n");
    }

    //Determine where Dracula is
    LocationID draculaLocation;
    draculaLocation = whereIs(gameState, PLAYER_DRACULA);

    //Loop through possibleDestinations to find a match for Dracula's location
    int i;
    for (i = 0; i < numLocations[0]; i++) {
        if (draculaLocation == possibleDestinations[i]) {
            registerBestPlay(idToAbbrev(possibleDestinations[i]), "Single Move...");
            return possibleDestinations[i];
        }
    }

    //DEBUG
    if (DEBUG) {
        printf("singleMove successful (invalid result)\n");
    }

    //Return if no match is found
    return UNKNOWN_LOCATION;
}


//Make a move that is the first step of the shortest path to dracula
LocationID shortestMove(HunterView gameState, LocationID possibleDestinations[], int *numLocations, Map europe)
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
    if (hunterLocation >= MIN_MAP_LOCATION && hunterLocation <= MAX_MAP_LOCATION && draculaLocation >= MIN_MAP_LOCATION && draculaLocation <= MAX_MAP_LOCATION) {

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
LocationID lastKnownMove(HunterView gameState, LocationID possibleDestinations[], int *numLocations, Map europe)
{
    //DEBUG
    if (DEBUG) {
        printf("lastKnownMove entered\n");
    }    

    //Initialise path[] and trans[]
    LocationID path[NUM_MAP_LOCATIONS];
    TransportID trans[NUM_MAP_LOCATIONS];

    //DEBUG
    if (DEBUG) {
        printf("path[] and trans[] initialised\n");
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
        if (draculaLocation >= MIN_MAP_LOCATION && draculaLocation <= MAX_MAP_LOCATION) {
            break;
        }
    }

    //DEBUG 
    if (DEBUG) {
        printf("draculaLocation is: %d\n", draculaLocation);
    }

    //Overcome bug of hunter getting stuck at a past location of Dracula's
    if (draculaLocation == hunterLocation) {
        return UNKNOWN_LOCATION;
    }

    //Overcome bug of hunter moving back to a location in his trail
    LocationID hunterTrail[TRAIL_SIZE];
    giveMeTheTrail(gameState, whoAmI(gameState), draculaTrail);
    int j;
    for (j = 0; j < TRAIL_SIZE; j++) {
        if (draculaLocation == hunterTrail[j]) {
            return UNKNOWN_LOCATION;
        }
    }

    //Return first step along the route to Dracula, or UNKNOWN_LOCATION if invalid
    if (hunterLocation >= MIN_MAP_LOCATION && hunterLocation <= MAX_MAP_LOCATION &&
        draculaLocation >= MIN_MAP_LOCATION && draculaLocation <= MAX_MAP_LOCATION) {

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



//Stay in the current location if health is low
//If hunter's health is below HEALTH_CRITICAL, then they will rest and regain 3 health
//Returns the locationID correpsonding to the Location Initials registered
LocationID healthCriticalMove(HunterView gameState) {

    //DEBUG
    if (DEBUG) {
        printf("healthCriticalMove entered\n");
    }

    //Determine where Player is
    LocationID hunterLocation;
    hunterLocation = whereIs(gameState, whoAmI(gameState));

    //Determine Player's health
    int health;
    health = howHealthyIs(gameState, whoAmI(gameState));

    //DEBUG
    if (DEBUG) {
        printf("hunterLocation is: %d; health is: %d\n", hunterLocation, health);
    }

    //Decide whether to make a move or not
    if (health <= HEALTH_CRITICAL) {
        registerBestPlay(idToAbbrev(hunterLocation), "Health Critical Move...");

        //DEBUG
        if (DEBUG) {
            printf("healthCriticalMove successful. Health Critical\n");
        }
        return hunterLocation;
    } 

    //DEBUG
    if (DEBUG) {
        printf("healthCriticalMove successful. Health Abundant\n");
    }

    //Return invalid result
    return UNKNOWN_LOCATION;

}



//Stay in the current location if research would be beneficial
//If Dracula's past location is unknown, and the trail is full then conduct research
//Returns the locationID correpsonding to the Location Initials registered
LocationID researchMove(HunterView gameState) {

    //DEBUG
    if (DEBUG) {
        printf("researchMove entered\n");
    }

    //Determine the length of the trail
    Round currentRound;
    currentRound = giveMeTheRound(gameState);
    //Abort now if trail too short
    if (currentRound <= TRAIL_SIZE) {
        return UNKNOWN_LOCATION;
    }

    //Determine where Player is
    LocationID hunterLocation;
    hunterLocation = whereIs(gameState, whoAmI(gameState));
    if (hunterLocation <= MIN_MAP_LOCATION || hunterLocation >= MAX_MAP_LOCATION) {
        return UNKNOWN_LOCATION;
    }

    //DEBUG
    if (DEBUG) {
        printf("hunterLocation is: %d\n", hunterLocation);
    }

    //Determine where Dracula was last known to be (if valid, end function)
    LocationID draculaLocation;
    LocationID draculaTrail[TRAIL_SIZE];
    giveMeTheTrail(gameState, PLAYER_DRACULA, draculaTrail);
    int i;
    for (i = 0; i < TRAIL_SIZE; i++) {
        draculaLocation = draculaTrail[i];
        if (draculaLocation >= MIN_MAP_LOCATION && draculaLocation <= MAX_MAP_LOCATION) {
            return UNKNOWN_LOCATION;
        }
    }

    //DEBUG 
    if (DEBUG) {
        printf("draculaLocation is: %d\n", draculaLocation);
    }

    //Execute research order
    registerBestPlay(idToAbbrev(hunterLocation), "Research Move...");

    //Return valid result
    return hunterLocation;

}

LocationID directCastleMove(HunterView gameState, LocationID possibleDestinations[], int *numLocations, Map europe) {

    //DEBUG
    if (DEBUG) {
        printf("directCastleMove entered\n");            
    }

    //Determine where Player is
    LocationID hunterLocation;
    hunterLocation = whereIs(gameState, whoAmI(gameState));

    //DEBUG
    if (DEBUG) {
        printf("hunterLocation is: %d\n", hunterLocation);
    }

    //If currently at Castle Dracula, stay there
    if (hunterLocation == CASTLE_DRACULA) {
        registerBestPlay("CD", "Staying at Mount Doom...");
        return hunterLocation;
    }

    //Otherwise, take the first step along the shortest route to the castle

    //Initialise path[] and trans[]
    LocationID path[NUM_MAP_LOCATIONS];
    TransportID trans[NUM_MAP_LOCATIONS];

    //DEBUG
    if (DEBUG) {
        printf("path[] and trans[] initialised\n");
    }

    //Call shortestPath
    shortestPath(europe, hunterLocation, CASTLE_DRACULA, path, trans);

    //DEBUG
    if (DEBUG) {
        printf("shortestPath called\n");
    }

    //Register result
    registerBestPlay(idToAbbrev(path[1]), "Direct route to Mount Doom...");

    //DEBUG
    if (DEBUG) {
        printf("hunterLocation is: %d; CASTLE_DRACULA is: %d\n", hunterLocation, CASTLE_DRACULA);
        printf("path[0] is: %d; path[1] is: %d\n", path[0], path[1]);
        printf("directCastleMove successful\n");            
    }

    //Return first step
    return path[1];

}
