#ifndef GAME_H
#define GAME_H




#include <iostream>

#include "helperFunctions.h"
#include "globals.h"
#include "Room.h"
#include "GameObject.h"
#include "Flashlight.h"
#include "Battery.h"
#include "Inventory.h"
#include "Solarium.h"
#include "Armory.h"
#include "CaptainsQuarters.h"
#include "Medbay.h"
#include "Player.h"
#include "EscapePod.h"
#include "AirlockControls.h"
#include "Parser.hpp"
#include "inputStruct.hpp"


enum locations { none = 0, room, player_inv };
enum exitCodes { hardExit = 5, quickLoad = 6};

class Game
{

protected:
	Player player;	//stores player inventory and current/previous room locations
	int gameNumber;

	//rooms
	/* test rooms
	Armory armory;
	Medbay medbay;
	Crewquarters crewquarters;
	std::vector<Room*> allRooms = {&armory, &medbay, &crewquarters};
	*/
	Room bridge, cargobay, commcenter, crewquarters, engineeringbay,
		galley, hydroponics, medbay, pods, recroom, researchlab, secoffice;

	Solarium solarium;
	CaptainsQuarters captainsquarters;
	Armory armory;

	std::vector<Room*> allRooms = {&armory, &bridge, &captainsquarters, &cargobay, &commcenter, 
							&crewquarters, &engineeringbay, &galley, &hydroponics, &medbay, &pods, 
							&recroom, &researchlab, &secoffice, &solarium};

	//objects
	//* test objects
	//Flashlight flashlight;
	//Battery battery;
	//*/
	GameObject autodoc, armorydoor, battery, birthdaycake, birthdayhat, captainsdesk, captskey, compterminal, diningtable, dirtydishes,
		flashlight_real, glasscabinets, keycard, medcorpse, medposter, o2monitor, paper, pdacharger, pen, photos, podai, ringofkeys, scrawlednote, seccamconsole, secChiefBody, secfootage,
		spaceapple, spacesuit, stationterminal, tomatoplant, workbench, baywindow, blueprint, c4, commsconsole, dcube, decondoor, electronscope,
		gaussrifle, labspecimen, laserarray, podcontrolboard, podnav, space, videomonitor, blastdoor, cargocrane, chiefengcorpse, cranekey, podgen,
		storageshelves, viewingwindow, cargoshelves, collapsedpassage, locker, plasmacutter, podframepanel, securitygate, charredphoto, podrepairmanual;

	AirlockControls airlockcontrols;
	EscapePod escapePod;
	
	std::vector<GameObject*> allObjects = { &airlockcontrols, &armorydoor, &autodoc, &battery, &birthdaycake, &birthdayhat, &captainsdesk, &captskey, &compterminal, &diningtable, &dirtydishes, 
				&escapePod, &flashlight_real, &glasscabinets, &keycard, &medcorpse, &medposter, &o2monitor, &paper, &pdacharger, &pen, &photos, &podai, &ringofkeys, &scrawlednote, &seccamconsole, &secChiefBody, &secfootage,
				&spaceapple, &spacesuit, &stationterminal, &tomatoplant, &workbench, &baywindow, &blueprint, &c4, &commsconsole, &dcube, &decondoor, &electronscope,
				&gaussrifle, &labspecimen, &laserarray, &podcontrolboard, &podnav, &space, &videomonitor, &blastdoor, &cargocrane, &chiefengcorpse, &cranekey, &podgen,
				&storageshelves, &viewingwindow,  &cargoshelves, &collapsedpassage, &locker, &plasmacutter, &podframepanel, &securitygate, &charredphoto, &podrepairmanual  };

	//track game status
	bool gameOver;
	

public:
	Game(int);
	//void displayMainMenu();
	void loadGameFiles(std::string);
	int playGame();
	

};



#endif