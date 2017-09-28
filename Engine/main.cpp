#include <string>
#include <vector>
#include <iostream>

#include "Game.h"
#include "GameManager.h"


#include "fileops.h"
#include "fileparse.h"
//main
int main()
{
	//saveGame("");


	//std::string dog = samsModule("../sg_savedgameslist", "rrtynertynertyn");
	//std::cout << dog;

	//dropObject("../sg_playerlocation", "END", "LOCATION");
	//dropObject("../sg_playerlocation", "nowhere", "LOCATION");

	//cin.get();

	//Game g;
	//g.displayMainMenu();

	GameManager gm;
	gm.initializeGame();
	
	/*cin.get();*/
	return 0;

}


