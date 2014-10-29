// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"
#include "Places.h"

int isLv2Move(DracView gameState, int move);
int isSafeFrom(DracView gameState, int move, int player);

void decideDraculaMove(DracView gameState)
{
	//Grabs the locations of all 4 hunters
	int player0 = whereIs(gameState, 0);
	int player1 = whereIs(gameState, 1);
	int player2 = whereIs(gameState, 2);
	int player3 = whereIs(gameState, 3);

	int *numLocations = malloc(sizeof(int));
	//assert(numLocations!=NULL);

	//if the location is in the trail
	int inTrail = FALSE;

	//contains the array of Dracula possible moves
	LocationID *possible = whereCanIgo(gameState, &numLocations[0], TRUE, FALSE);
	
	int i = 0;
	int j = 0;

	// move where no hunter is and no hunter can get to
	int move = UNKNOWN_LOCATION;
	// move where no hunter is
	int secondbest = UNKNOWN_LOCATION; 
	
	LocationID *trail = malloc(TRAIL_SIZE*sizeof(int));
	giveMeTheTrail(gameState, PLAYER_DRACULA, trail);
	
	while(move == UNKNOWN_LOCATION && i < numLocations[0]){
		inTrail = FALSE;
		if (possible[i]!= player0 && possible[i]!= player1 && 
				possible[i]!= player2 && possible[i]!=player3){

			for(j=0;j<TRAIL_SIZE;j++){
				if (possible[i] == trail[j]){
					inTrail = TRUE;
				}
			}

			if (!inTrail){
				int type = idToType(possible[i]);
				if (type != SEA){
					secondbest = possible[i];
					if (isLv2Move(gameState, secondbest)){
						move = secondbest;
					}
				}
			}
		}
		i++;
	}

//-----------------------------LOADING BEST MOVE INTO VAR------------------------	
	//Initialises the randomisation
	srand(time(NULL));
	int r = rand()%2;
	
	if (move == UNKNOWN_LOCATION){
		move = secondbest;
		if (move == UNKNOWN_LOCATION){
			if (r == 1){ 
				registerBestPlay("TP","Hello my delicious");
			} else if (r == 0 && inTrail == 0){
				registerBestPlay("TP","Hello my delicacy");		
			} else if (r == 0 && inTrail == 1){
				registerBestPlay("TP","Hello my dinner");
			}
		} else {
			char *moveAbbrev = idToAbbrev(move);
			registerBestPlay(moveAbbrev,"Hello my darlings");
		}
	} else {
		char *moveAbbrev = idToAbbrev(move);
		registerBestPlay(moveAbbrev,"Hello my dears");
	}

}

int isLv2Move(DracView gameState, int move){
	int safe = FALSE;
	if (isSafeFrom(gameState, move, 0)){
		if(isSafeFrom(gameState, move, 1)){
			if(isSafeFrom(gameState, move, 2)){
				if(isSafeFrom(gameState, move, 3)){
					safe = TRUE;
				}
			}
		}
	}
	return safe;
}

int isSafeFrom(DracView gameState, int move, int player){

	int safe = TRUE;
	int j;
	int *numLoc = malloc(sizeof(int));

	//arrays of places the hunters can go
	LocationID *playerCan = whereCanTheyGo(gameState, &numLoc[0], 0, TRUE, TRUE, TRUE);

	for (j=0; j<numLoc[0]; j++){
		if(move == playerCan[j]){
			safe = FALSE;
			break;
		}
	}

	free(numLoc);
	return safe;
}