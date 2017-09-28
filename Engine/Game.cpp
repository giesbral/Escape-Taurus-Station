#include "Game.h"
#include <sstream>
#include <iterator>
#include <limits>
#include <unordered_set>
#include <map>
#include "fileparse.h"
#include "fileops.h"
#include <fstream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <time.h>

//prototypes
void printNearbyItems(Player*);
void printItemsInObject(GameObject* g);
void loadRoomConnectionAliases(std::string targetFile, std::string direction, std::vector<std::string> &directionAliases);
Room* loadRoomConnections(std::string targetFile, std::string direction, const std::vector<Room*> &allRooms);
locations getItemLocation(std::string noun, const Inv &playerInventory, Inv roomObjects);
void loadRoomObjects(std::string targetFile, const std::vector<GameObject*> &allObjects, Room* &room);
void loadObjectObjects(std::string targetFile, const std::vector<GameObject*> &allObjects, GameObject* &object);
void loadObjectAliases(std::string targetFile, GameObject* object);
std::string inputToBaseName(std::string input, Inv roomInv, Inv playerInv);
void addToInvAndRemoveFromRoom(Player &player, GameObject* selectedGameObject, GameObject* selectedGameObject2);
bool loadRoomLocks(std::string targetFile, std::string direction); //if we add lockstatus header, read it instead of lock header (that heads the lock desciption)
void printHelpScreen();
void lookAtDirection(std::string selectedDirectionString, Player& player, std::string usersDirectionInput);
void loadPlayerInventory(std::string targetFile, const std::vector<GameObject*> &allObjects, Player& player);
void loadPodMissingParts(std::string targetFile, const std::vector<GameObject*> &allObjects, EscapePod &object);

Game::Game(int n)
{
	gameNumber = n;
}


void Game::loadGameFiles(std::string sourceFolder)
{
	sourceFolder += "/";
	
	std::string targetFile;

	std::string tempString;

	std::string tempRoomString;
	std::string tempRoomString1;

	std::vector<std::string> allRoomDataFiles = {"ARMORY", "BRIDGE", "CAPTQUARTERS", "CARGOBAY", "COMMCENTER", 
					"CREWQUARTERS", "ENGINEERINGBAY", "GALLEY", "HYDROPONICS", "MEDBAY", "PODS", 
					"RECROOM", "RESEARCHLAB", "SECOFFICE", "SOLARIUM"};
	
	std::vector<std::string> allObjectDataFiles = { "AIRLOCKCONTROLS", "ARMORYDOOR", "AUTODOC", "BATTERY", "BIRTHDAYCAKE", "BIRTHDAYHAT", "CAPTAINSDESK",
													"CAPTAINSKEY", "COMPTERMINAL", "DININGTABLE", "DIRTYDISHES", "BROKENESCAPEPOD", "FLASHLIGHT", "GLASSCABINETS", "KEYCARD", "MEDCORPSE", "MEDPOSTER", "OTWOMONITOR", "PAPER", "PDACHARGER", "PEN", "PHOTOS", "PODAI", "KEYRING",
													"SCRAWLEDNOTE", "SECURITYCAMERACONSOLE", "SECURITYCHIEFSCORPSE", "SECURITYFOOTAGE",
													"SPACEAPPLE", "SPACESUIT", "STATIONTERMINAL", "TOMATOPLANT", "WORKBENCH", "BAYWINDOW", "BLUEPRINT",
													"CFOUR", "COMMSCONSOLE", "DCUBE", "DECONDOOR", "ELECTRONSCOPE", "GAUSSRIFLE", "LABSPECIMEN", "LASERARRAY",
													"PODCONTROLBOARD", "PODNAV", "SPACE", "VIDEOMONITOR", "BLASTDOOR", "CARGOCRANE", "CHIEFENGINEERSCORPSE",
													"CRANEKEY", "PODGENERATOR", "STORAGESHELVES", "VIEWINGWINDOW", "CARGOSHELVES", "COLLAPSEDPASSAGE", "LOCKER", "PLASMACUTTER", "PODFRAMEPANEL", "SECURITYGATE", "CHARREDPHOTO", "PODREPAIRMANUAL"};
	
	//std::cout << allObjects.size() << " " << allObjectDataFiles.size();
	//cin.get();
	if (allObjects.size() != allObjectDataFiles.size())
		exit(90);
	if (allRooms.size() != allRoomDataFiles.size())
		exit(91);

	//load object data  NOTE: sourceFolder may or may not be needed depending on implementation of save state
	for (int i = 0; i < allObjects.size(); i++)
	{
		//set target file

		targetFile = "../" + sourceFolder + allObjectDataFiles[i] ;	//flip

		allObjects[i]->setDataFilename(targetFile);								//datafilename

		allObjects[i]->setHeaderName(allObjectDataFiles[i]);					//headername

		allObjects[i]->setName(samsModule(targetFile, "OBJECTNAME"));			//name

		loadObjectAliases(targetFile, allObjects[i]);							//load aliases for the object

		allObjects[i]->setUseableWith(samsModule(targetFile, "USABLE WITH"));	//useable with (gets set to "END" or "" if not useable with another item)

		tempString = samsModule(targetFile, "LOOTABLE");						//lootable
		if (tempString == "True" || tempString == "true")
			allObjects[i]->setLootable(true);
		else
			allObjects[i]->setLootable(false);

	}

	//load objects in other objects (i.e. dish in cuboard)
	for (int i = 0; i < allObjects.size(); i++)
	{
		//set target file
		targetFile = "../" + sourceFolder + allObjectDataFiles[i] ;	//flip
		loadObjectObjects(targetFile, allObjects, allObjects[i]);
	}

	//load ready/activated and worn item states from file
	std::string targetReadyFile = "../" + sourceFolder + "sg_itemsready";
	std::string targetActivatedFile = "../" + sourceFolder + "sg_itemsactivated";
	std::string targetWornFile = "../" + sourceFolder + "sg_itemsworn";
	for (int i = 0; i < allObjects.size(); i++)
	{
		std::string itemName = allObjects[i]->getName();
		std::string itemSearch = samsModule(targetReadyFile, itemName);
		if (itemSearch != "END" && itemSearch != "")
		{
			allObjects[i]->setReady(true);
			pickUpObject(targetReadyFile, itemName, "OBJECTS");
		}
		itemSearch = samsModule(targetActivatedFile, itemName);
		if (itemSearch != "END" && itemSearch != "")
		{
			allObjects[i]->setActivated(true);
			pickUpObject(targetActivatedFile, itemName, "OBJECTS");
		}
		itemSearch = samsModule(targetWornFile, itemName);
		if (itemSearch != "END" && itemSearch != "")
		{
			allObjects[i]->setBeingWornByPlayer(true);
			pickUpObject(targetWornFile, itemName, "OBJECTS");
		}
	}

	//load player's inventory
	targetFile = "../" + sourceFolder + "sg_inventory";
	player.setInventoryDatafileName(targetFile);
	loadPlayerInventory(targetFile, allObjects, player);

	//load room data (names, descriptions, objects, exit aliases, and visisted flag)
	for (int i = 0; i < allRooms.size(); i++)
	{
		//set target file

		targetFile = "../" + sourceFolder + allRoomDataFiles[i] ;	//flip

		//load room name and descriptions
		allRooms[i]->setName(samsModule(targetFile, "ROOMNAME"));
		allRooms[i]->setLongDescription(samsModule(targetFile, "LONGFORM"));
		allRooms[i]->setShortDescription(samsModule(targetFile, "SHORTFORM"));
		allRooms[i]->setDataFilename(targetFile);

		//load playerbeenherebefore data
		std::string tempVisited = samsModule(targetFile, "VISITED");
		if (tempVisited == "true" | tempVisited == "True")
			allRooms[i]->setPlayerHasBeenHereBefore(true);
		else
			allRooms[i]->setPlayerHasBeenHereBefore(false);

		//load direction aliases
		loadRoomConnectionAliases(targetFile, "FORWARD", allRooms[i]->northAliases);
		loadRoomConnectionAliases(targetFile, "AFT", allRooms[i]->southAliases);
		loadRoomConnectionAliases(targetFile, "PORT", allRooms[i]->westAliases);
		loadRoomConnectionAliases(targetFile, "STARBOARD", allRooms[i]->eastAliases);

		//load room objects
		loadRoomObjects(targetFile, allObjects, allRooms[i]);
	}
	
	//load room connections (including whether that connection is locked or not)
	for (int i = 0; i < allRooms.size(); i++)
	{
		//set target file

		targetFile = "../"  + sourceFolder + allRoomDataFiles[i];	//flip

		allRooms[i]->North = loadRoomConnections(targetFile, "FORWARD", allRooms);	
		allRooms[i]->South = loadRoomConnections(targetFile, "AFT", allRooms);	
		allRooms[i]->West = loadRoomConnections(targetFile, "PORT", allRooms);	
		allRooms[i]->East = loadRoomConnections(targetFile, "STARBOARD", allRooms);	

		allRooms[i]->lockedNorth = loadRoomLocks(targetFile, "FORWARD");
		allRooms[i]->lockedSouth = loadRoomLocks(targetFile, "AFT");
		allRooms[i]->lockedWest = loadRoomLocks(targetFile, "PORT");
		allRooms[i]->lockedEast = loadRoomLocks(targetFile, "STARBOARD");
	}

	//load player's starting location
	std::string startingRoom = samsModule("../" + sourceFolder + "sg_playerlocation", "LOCATION");
	for (int i = 0; i < allRooms.size(); i++)
	{
		if (startingRoom == allRooms[i]->getName())
		{
			player.setPlayerCurrentRoom(allRooms[i]);
			break;
		}
	}


	//add missing parts to escape pod
	loadPodMissingParts("../" + sourceFolder + "BROKENESCAPEPOD", allObjects, escapePod);
	
	//load reachability status of the podgenertaor
	if (storageshelves.getReady() == true)
		podgen.setReachable(true);
	else
		podgen.setReachable(false);

	//testing purposes only
	//escapePod.addMissingPart(&birthdayhat); //testing only
	//escapePod.addMissingPart(&pen);
	//crewquarters.addItem(&spacesuit);

	//player.setPlayerCurrentRoom(&secoffice);
	//player.addItem(&plasmacutter, false);
	//player.addItem(&podai, false);
	//player.addItem(&podcontrolboard, false);
	//player.addItem(&podgen, false);
	//player.addItem(&podnav, false);
	//player.addItem(&podframepanel, false);
}


//void Game::displayMainMenu()
//{
//	char menuChoice;
//	std::string saveGameChoice;
//
//	std::cout << std::string( 100, '\n' );
//	
//	centerstring(" ______     ______     ______     ______     ______   ______          \n");
//	centerstring("/\\  ___\\   /\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\ /\\  ___\\         \n");
//	centerstring("\\ \\  __\\   \\ \\___  \\  \\ \\ \\____  \\ \\  __ \\  \\ \\  _-/ \\ \\  __\\         \n");
//	centerstring(" \\ \\_____\\  \\/\\_____\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_\\    \\ \\_____\\       \n");
//	centerstring("  \\/_____/   \\/_____/   \\/_____/   \\/_/\\/_/   \\/_/     \\/_____/       \n");
//	centerstring(" ______   ______     __  __     ______     __  __     ______          \n");
//	centerstring("/\\__  _\\ /\\  __ \\   /\\ \\/\\ \\   /\\  == \\   /\\ \\/\\ \\   /\\  ___\\         \n");
//	centerstring("\\/_/\\ \\/ \\ \\  __ \\  \\ \\ \\_\\ \\  \\ \\  __<   \\ \\ \\_\\ \\  \\ \\___  \\        \n");
//	centerstring("   \\ \\_\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\/\\_____\\       \n");
//	centerstring("    \\/_/   \\/_/\\/_/   \\/_____/   \\/_/ /_/   \\/_____/   \\/_____/       \n");
//	centerstring(" ______     ______   ______     ______   __     ______     __   __    \n");
//	centerstring("/\\  ___\\   /\\__  _\\ /\\  __ \\   /\\__  _\\ /\\ \\   /\\  __ \\   /\\ \"-.\\ \\   \n");
//	centerstring("\\ \\___  \\  \\/_/\\ \\/ \\ \\  __ \\  \\/_/\\ \\/ \\ \\ \\  \\ \\ \\/\\ \\  \\ \\ \\-.  \\  \n");
//	centerstring(" \\/\\_____\\    \\ \\_\\  \\ \\_\\ \\_\\    \\ \\_\\  \\ \\_\\  \\ \\_____\\  \\ \\_\\\\\"\\_\\ \n");
//	centerstring("   \\/_____/     \\/_/   \\/_/\\/_/     \\/_/   \\/_/   \\/_____/   \\/_/ \\/_/ \n\n");
//
//	centerstring("Main Menu\n\n");
//	centerstring("1. New Game \n");
//    centerstring("2. Load Game \n");                                                                
//	centerstring("3. Help     \n");  
//	centerstring("4. Exit     \n\n");  
//
//	centerstring("Enter a number from the Main Menu list: ");
//
//	std::cin.get(menuChoice);
//	switch (menuChoice)
//	{
//		case 49:			//decimal value for ascii char '1'
//			std::cout << std::string(100, '\n');
//			std::cout << "Press Enter to begin your new game.\n";
//			std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
//			std::cin.get();
//			loadGameFiles("");
//			std::cout << std::string(100, '\n');
//			playGame();
//			break;
//		case 50:			//decimal value for ascii char '2'
//			std::cout << std::string(100, '\n');
//			std::cout << "Load Game functionality had not been implemented yet.\n";
//			std::cout << "Press Enter to return to the Main Menu\n";
//			std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
//			std::cin.get();
//			displayMainMenu();
//			break;
//			/*
//			std::cout << std::string(100, '\n');
//			std::cout << "Enter the number of the saved game you want to load.\n";	//NOTE:: add a list of saved games
//			std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
//			std::cin >> saveGameChoice;
//			std::cout << "Loading save game " << saveGameChoice << std::endl;		//NOTE:: add a check to verify valid selection
//			std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
//			std::cin.get();
//			loadGameFiles(saveGameChoice);
//			std::cout << std::string(100, '\n');
//			playGame();
//			break;
//			*/
//		case 51: 
//			std::cout << std::string(100, '\n');
//			printHelpScreen();
//			std::cout << std::endl;
//			std::cout << "Press Enter to return to the Main Menu\n";
//			std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
//			std::cin.get();
//			displayMainMenu();
//			break;
//		case 52: 
//			std::cout << std::string(100, '\n');
//			std::cout << "Are you sure you want to Exit? (Enter Y to exit)\n";
//			std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
//			std::cin.get(menuChoice);
//			if (menuChoice == 'Y' || menuChoice == 'y')
//				break;
//			else
//				displayMainMenu();
//			break;
//		default:
//			displayMainMenu();
//	}
//
//}


std::string getUserInput()
{
	//get userinput
	//std::cout << "\nWhat would you like to do? format: <action> <object>" << std::endl;
	std::cout << "\n>";
	
	std::string userInput;
	
	std::getline(std::cin, userInput);

	return userInput;
}


int Game::playGame()
{
	//variables
	locations itemLocation;
	locations item2Location;
	
	std::vector<std::string> itemStrings;
	for (int i = 0; i < allObjects.size(); i++)
		itemStrings.push_back(stringToLowercase(allObjects[i]->getName()));

	std::vector<std::string>::iterator it;
	
	int numInputObjects;

	bool canMoveDirection;
	bool validDirection;

	GameObject* selectedGameObject = NULL;
	GameObject* selectedGameObject2 = NULL;
	Room* selectedDirection = NULL;
	std::string selectedDirectionString = "";

	//holds parsed user input
	inputStruct parsedInput;

	//print description of starting room (or where player resumes saved game)
	std::cout << player.getPlayerCurrentRoom()->getName() << std::endl;
	std::cout << player.getPlayerCurrentRoom()->look(&player) << std::endl;
	player.getPlayerCurrentRoom()->setPlayerHasBeenHereBefore(true);
	//printNearbyItems(&player);
	if (player.getPlayerCurrentRoom()->canSeeObjects(&player) == true)
		printNearbyItems(&player);

	//for random testing
	bool ranTesting = false;
	int counter = 0 ;
	srand (time(NULL));
	int ranNum;
	std::vector<std::string> allNounInputs;
	std::vector<std::string> allDirections{"forward","aft", "port","starboard"};
	allNounInputs = allDirections;	//add all direction to vector
	for (int i = 0; i < allObjects.size(); i++)	//add all object names to vector
		allNounInputs.push_back(stringToLowercase(allObjects[i]->getName()));

	Verbs commandExit = No_Input;
	while (commandExit != Exit) //loop getting user input and trigger actions
	{
		if (ranTesting == false)
		{
			parsedInput = parser::parseInput(getUserInput()); //get user input 
		}
		else
		{
			counter++;

			parsedInput.nouns.clear();
			ranNum = rand() % allNounInputs.size();
			parsedInput.nouns.push_back(stringToLowercase(allNounInputs[ranNum]));	//add random first noun

			if (rand() % 2 == 1)	//50% chance to add random second noun
			{
				ranNum = rand() % allNounInputs.size();
				parsedInput.nouns.push_back(stringToLowercase(allNounInputs[ranNum]));
			}

			std::vector<Verbs> theverbs{ Go, Take,Use,Combine, Ignite, Wear, Search};
			std::vector<std::string> theverbstring{ "go", "Take","Use","Combine", "Ignite", "Wear", "Search"};

			ranNum = rand() % theverbs.size();
			parsedInput.verb = theverbs[ranNum];	//set a random verb
			parsedInput.verbInput = theverbstring[ranNum];

			//ranNum = rand() % 50;	//random chance to chnage the players room
			//if (ranNum < 5)
			//{
			//	ranNum = rand() % allRooms.size();
			//	player.setPlayerCurrentRoom(allRooms[ranNum]);
			//}

			//if (counter == 500000 - 1)
			//{
			//	std::cout << "counter: " << counter << std::endl;
			//	parsedInput.verb = Inventory;
			//}
			//else if (counter == 500000)
			//{
			//	std::cout << "counter: " << counter << std::endl;
			//	parsedInput.verb = Save;
			//}
			//else if (counter > 500000)
			//{
			//	std::cout << "counter: " << counter << std::endl;
			//	parsedInput.verb = Exit;
			//}

			if (parsedInput.nouns.size() > 1)
				std::cout << std::endl << parsedInput.verbInput << " " << parsedInput.nouns[0] << " " << parsedInput.nouns[1] << std::endl;
			else
				std::cout << std::endl << parsedInput.verbInput << " " << parsedInput.nouns[0] << std::endl;
			//cin.get();
		}

		//debug print input
		/*
		std::cout << "verb value: " << parsedInput.verb << std::endl;
		std::cout << "verb: " << parsedInput.verbInput << std::endl;
		std::cout << "noun(s): " << std::endl;
		for (int i = 0; i < parsedInput.nouns.size(); i++)
			std::cout << parsedInput.nouns[i] << std::endl;
		*/

		//find the direction input by user
		selectedDirectionString = "invalid";
		if (parsedInput.nouns.size() > 0 && (parsedInput.verb == Go || parsedInput.verb == Invalid || parsedInput.verb == Look_At))
		{
			it = find(player.getPlayerCurrentRoom()->northAliases.begin(), player.getPlayerCurrentRoom()->northAliases.end(), parsedInput.nouns[0]);
			if (it != player.getPlayerCurrentRoom()->northAliases.end())
				selectedDirectionString = player.getPlayerCurrentRoom()->northAliases[0];
			
			if (selectedDirectionString == "invalid")
			{
				it = find(player.getPlayerCurrentRoom()->southAliases.begin(), player.getPlayerCurrentRoom()->southAliases.end(), parsedInput.nouns[0]);
				if (it != player.getPlayerCurrentRoom()->southAliases.end())
					selectedDirectionString = player.getPlayerCurrentRoom()->southAliases[0];
			}

			if (selectedDirectionString == "invalid")
			{
				it = find(player.getPlayerCurrentRoom()->eastAliases.begin(), player.getPlayerCurrentRoom()->eastAliases.end(), parsedInput.nouns[0]);
				if (it != player.getPlayerCurrentRoom()->eastAliases.end())
					selectedDirectionString = player.getPlayerCurrentRoom()->eastAliases[0];
			}

			if (selectedDirectionString == "invalid")
			{
				it = find(player.getPlayerCurrentRoom()->westAliases.begin(), player.getPlayerCurrentRoom()->westAliases.end(), parsedInput.nouns[0]);
				if (it != player.getPlayerCurrentRoom()->westAliases.end())
					selectedDirectionString = player.getPlayerCurrentRoom()->westAliases[0];
			}
		}

		//look at a direction (or direction alias)
		if (selectedDirectionString != "invalid" && parsedInput.verb == Look_At)
		{
			lookAtDirection(selectedDirectionString, player, parsedInput.nouns[0]);
			continue;
		}

		//process command
		if (parsedInput.verb == Invalid && selectedDirectionString == "invalid")	//Invalid comand
		{
			std::cout << "I don't understand your command." << std::endl;
			continue;
		}
		if (parsedInput.verb == No_Input)	//No input comand
		{
			std::cout << "You have to input something." << std::endl;
			continue;
		}
		else if (parsedInput.verb == Menu)	// Menu
		{
			
			std::string choice;
			std::cout << "Are you sure you want to return to Menu? (Enter Y to go to the Menu)\n";
			std::getline(std::cin, choice);
			if (choice == "Y" || choice == "y")
			{
				//displayMainMenu();
				commandExit = Exit;
			}
			else
			{
				std::cout << "Game resumed." << std::endl;
				continue;
			}

			break;
		}
		else if (parsedInput.verb == Exit)	//Exit
		{
			std::string choice;
			std::cout << "Are you sure you want to Exit? (Enter Y to exit)\n";
			std::getline(std::cin, choice);
			if (choice == "Y" || choice == "y")
			{
				std::cout << "Exiting.\n";
				return hardExit;
				//commandExit = Exit;
			}
			else
			{
				std::cout << "Game resumed." << std::endl;
				continue;
			}
		}
		else if (parsedInput.verb == Go || parsedInput.verb == Invalid) //movement 
		{
			validDirection = true;
			if (parsedInput.nouns.size() == 0)
			{
				std::cout << "You must specify which way you want to go." << std::endl;
				continue;
			}
			else if (selectedDirectionString == "north")
			{
				selectedDirection = player.getPlayerCurrentRoom()->North;
				canMoveDirection = player.getPlayerCurrentRoom()->goDirection(selectedDirectionString, &player);	//note: passing string instead of Room* due to how goDirection() compares the parameter
			}
			else if (selectedDirectionString == "south")
			{
				selectedDirection = player.getPlayerCurrentRoom()->South;
				canMoveDirection = player.getPlayerCurrentRoom()->goDirection(selectedDirectionString, &player);
			}
			else if (selectedDirectionString == "east")
			{
				selectedDirection = player.getPlayerCurrentRoom()->East;
				canMoveDirection = player.getPlayerCurrentRoom()->goDirection(selectedDirectionString, &player);
			}
			else if (selectedDirectionString == "west")
			{
				selectedDirection = player.getPlayerCurrentRoom()->West;
				canMoveDirection = player.getPlayerCurrentRoom()->goDirection(selectedDirectionString, &player);
			}
			else
			{
				canMoveDirection = false;
				validDirection = false;
			}
			if (validDirection == false)	//invalid direction, don't move player
				std::cout << "You can't go that way (unrecognized direction)." << std::endl;
			else if (canMoveDirection == true && selectedDirection != NULL)	//valid direction, move player
			{
				player.setPlayerPreviousRoom(player.getPlayerCurrentRoom());
				player.setPlayerCurrentRoom(selectedDirection);

				if (player.getPlayerCurrentRoom()->getPlayerHasBeenHereBefore() == false)
				{
					std::cout << player.getPlayerCurrentRoom()->getName() << std::endl;
					//std::cout << player.getPlayerCurrentRoom()->getLongDescription() << std::endl;
					std::cout << player.getPlayerCurrentRoom()->look(&player) << std::endl;
					player.getPlayerCurrentRoom()->setPlayerHasBeenHereBefore(true);
					//printNearbyItems(&player);
					if (player.getPlayerCurrentRoom()->canSeeObjects(&player) == true)
						printNearbyItems(&player);
				}
				else
				{
					std::cout << player.getPlayerCurrentRoom()->getName() << std::endl;
					if (player.getPlayerCurrentRoom()->canSeeObjects(&player) == true)
					{
						std::cout << player.getPlayerCurrentRoom()->getShortDescription() << std::endl;
						printNearbyItems(&player);
					}
					else
						std::cout << player.getPlayerCurrentRoom()->look(&player) << std::endl;
				}
			}
			/*
			else
			{
				std::cout << "You're still in the " << playerCurrentRoom->getName() << std::endl;
			}
			*/

		}
		else if (parsedInput.verb == Inventory) //inventory
		{
			player.getPlayerInventory().printInventory();
		}
		else if (parsedInput.verb == Look) //look (print long room description)
		{
			std::cout << player.getPlayerCurrentRoom()->getName() << std::endl;
			std::cout << player.getPlayerCurrentRoom()->look(&player) << std::endl;
			if (player.getPlayerCurrentRoom()->canSeeObjects(&player) == true)
				printNearbyItems(&player);
		}
		else if (parsedInput.verb == Help) //help
		{
			printHelpScreen();

		}
		else if (parsedInput.verb == Save) //savegame
		{
			//save the game and store the save number to datafile
			std::string saveName = to_string(saveGame(to_string(gameNumber)));
			dropObject("../sg_savedgameslist", saveName, "STARTOFLIST");
			
			//save player's location to datafile
			dropObject("../" + saveName + "/sg_playerlocation", "END", "LOCATION");
			dropObject("../" + saveName + "/sg_playerlocation",  player.getPlayerCurrentRoom()->getName(), "LOCATION");
			
			//save ready, activated, and worn items to datafiles
			for (int i = 0; i < allObjects.size(); i++)
			{
				if (allObjects[i]->getReady() == true)
					dropObject("../" + saveName + "/sg_itemsready", allObjects[i]->getName(), "OBJECTS");
				if (allObjects[i]->getActivated() == true)
					dropObject("../" + saveName + "/sg_itemsactivated", allObjects[i]->getName(), "OBJECTS");
				if (allObjects[i]->getBeingWornByPlayer() == true)
					dropObject("../" + saveName + "/sg_itemsworn", allObjects[i]->getName(), "OBJECTS");
			}
			
			//print the save game code
			std::cout << "Game saved. Your saved game code is: " << saveName << "." << std::endl;
			std::cout << "Enter this number when you choose Load Game from Main Menu or you use the loadgame command." << std::endl;
		}
		else if (parsedInput.verb == Load) //loadgame
		{
			std::string choice;
			std::cout << "Are you sure you want to load a saved game? (Enter Y or N)\n";
			std::getline(std::cin, choice);
			if (choice == "Y" || choice == "y")
			{
				return quickLoad;
			}
			else
				std::cout << "Resuming current game.\n";
		}
		else //verb and object (one or two objects)
		{
			
			numInputObjects = parsedInput.nouns.size();

			std::string selectedObject1_BaseString = "";
			std::string selectedObject2_BaseString = "";
			
			if (numInputObjects == 0)
			{
				std::cout << "You didn't tell me what you want to " << parsedInput.verbInput << "." << std::endl;
				continue;
			}
			
			//get location(s) of object(s)
			if (numInputObjects > 0)
			{
				selectedObject1_BaseString = inputToBaseName(parsedInput.nouns[0], player.getPlayerCurrentRoom()->getRoomInventory(), player.getPlayerInventory());
				itemLocation = getItemLocation(selectedObject1_BaseString, player.getPlayerInventory(), player.getPlayerCurrentRoom()->roomObjects);
				item2Location = none;
			}
			if (numInputObjects > 1)
			{
				selectedObject2_BaseString = inputToBaseName(parsedInput.nouns[1], player.getPlayerCurrentRoom()->getRoomInventory(), player.getPlayerInventory());
				item2Location = getItemLocation(selectedObject2_BaseString, player.getPlayerInventory(), player.getPlayerCurrentRoom()->roomObjects);
			}
			
			//print "can't use" message if object isn't accessible by the player
			if (itemLocation == none)
			{
				std::cout << parsedInput.nouns[0] << " is not in your inventory or near you." << std::endl;
			}
			else if (item2Location == none && numInputObjects > 1)
			{
				std::cout << parsedInput.nouns[1] << " is not in your inventory or near you." << std::endl;
			}
			else  //interacting with objects
			{				
				selectedGameObject = selectedGameObject2 = NULL;
				for (int i = 0; i < itemStrings.size(); i++)
					if (selectedObject1_BaseString == itemStrings[i]) 
					{
						selectedGameObject = allObjects[i];
						break;
					}
				if (numInputObjects > 1)
				{
					for (int i = 0; i < itemStrings.size(); i++)
						if (selectedObject2_BaseString == itemStrings[i])
						{
							selectedGameObject2 = allObjects[i];
							break;
						}
				}
				//parse userinput (verb) and trigger action
				if (selectedGameObject != NULL)
				{
					if (parsedInput.verb == Use)	//use
					{		
						addToInvAndRemoveFromRoom(player, selectedGameObject, selectedGameObject2);
						selectedGameObject->use(selectedGameObject2, &player);
						if (escapePod.getActivated() == true)
						{
							std::cout << "\nPress Enter when ready.";		//should probably stick this stuff somewhere else
							std::cin.get();
							std::cout << std::string(100, '\n');
							std::this_thread::sleep_for(std::chrono::milliseconds(1000));
							centerstring("Escape Taurus Station\n\n");
							std::this_thread::sleep_for(std::chrono::milliseconds(500));
							centerstring("by\n\n");
							std::this_thread::sleep_for(std::chrono::milliseconds(500));
							centerstring("Alexander Giesbrecht\n");
							std::this_thread::sleep_for(std::chrono::milliseconds(500));
							centerstring("Alex Samuel\n");
							std::this_thread::sleep_for(std::chrono::milliseconds(500));
							centerstring("Patrick O'Larey\n");
							for (int i = 0; i < 28; i++)
							{
								std::cout << "\n";
								std::this_thread::sleep_for(std::chrono::milliseconds(500));
							}
							//std::cout << std::string(10, '\n');
							centerstring("Press Enter to return to Main Menu.");
							std::cin.get();
							//displayMainMenu();
							break;
						}
					}
					else if (parsedInput.verb == Combine)	//Combine
					{		
						addToInvAndRemoveFromRoom(player, selectedGameObject, selectedGameObject2);
						selectedGameObject->combine(selectedGameObject2, &player);
					}
					else if (parsedInput.verb == Consume)	 //consume
					{
						addToInvAndRemoveFromRoom(player, selectedGameObject, selectedGameObject2);
						selectedGameObject->consume(selectedGameObject2, &player);
					}
					else if (parsedInput.verb == Ignite)	 //ignite
					{
						addToInvAndRemoveFromRoom(player, selectedGameObject, selectedGameObject2);
						selectedGameObject->ignite(selectedGameObject2, &player);
					}
					else if (parsedInput.verb == Read)	 //read
					{
						addToInvAndRemoveFromRoom(player, selectedGameObject, selectedGameObject2);
						selectedGameObject->read(selectedGameObject2, &player);
					}
					else if (parsedInput.verb == Search)	 //search
					{
						addToInvAndRemoveFromRoom(player, selectedGameObject, selectedGameObject2);
						selectedGameObject->search(selectedGameObject2, &player);
					}
					else if (parsedInput.verb == Wear)		//wear
					{
						addToInvAndRemoveFromRoom(player, selectedGameObject, selectedGameObject2);	//maybe don't have an unsuccessful "wear" add the item to inventory
						selectedGameObject->wear(selectedGameObject2, &player);
					}
					else if (parsedInput.verb == Look_At)	//look at
					{
						selectedGameObject->lookAt(selectedGameObject2);
						printItemsInObject(selectedGameObject);
						if (selectedGameObject->getName() == "Broken Escape Pod")
							escapePod.printMissingParts();
					}
					else if (parsedInput.verb == Take) //take  note: only picks up the first object in the case of 2+ objects
					{
						if (itemLocation == player_inv)
						{
							std::cout << parsedInput.nouns[0] << " is already in your inventory." << std::endl;
						}
						else if (itemLocation == room)	//remove the object from the room if it was picked up from room
						{
							std::string takeText = samsModule(selectedGameObject->getDataFilename(), "TAKE");
							if (selectedGameObject->getLootable() == true && selectedGameObject->getReachable() == true)
							{
								addToInvAndRemoveFromRoom(player, selectedGameObject, NULL);
								if (takeText == "END" || takeText == "")	//if no special TAKE text: print standard message
									std::cout << selectedGameObject->getName() << " added to inventory." << std::endl;
								else
									std::cout << takeText << std::endl;
								//player.getPlayerCurrentRoom()->roomObjects.removeItem(selectedGameObject);
							}
							else if (selectedGameObject->getLootable() == true && selectedGameObject->getReachable() == false)
							{
								std::cout << "You can't reach the " << parsedInput.nouns[0] << " from here." << std::endl;
							}
							else
							{
								if (takeText == "END" || takeText == "") //if no special TAKE text: print standard message
									std::cout << "You can't add " << parsedInput.nouns[0] << " to your inventory." << std::endl;
								else
									std::cout << takeText << std::endl;
							}
						}
					}
					else if (parsedInput.verb == Drop)  //drop note: only drops the first object in the case of 2+ objects
					{
						if (itemLocation == player_inv)
						{
							player.removeItem(selectedGameObject);	//remove item from inv
							selectedGameObject->setBeingWornByPlayer(false);
							player.getPlayerCurrentRoom()->roomObjects.addItem(selectedGameObject); //add item to room
							std::cout << selectedGameObject->getName() << " dropped from inventory." << std::endl;
						}
						else if (itemLocation == room)
							std::cout << parsedInput.nouns[0] << " is not in your inventory." << std::endl;
					}
				}
				selectedGameObject = NULL;
				selectedGameObject2 = NULL;
			}
		}
	}


}


locations getItemLocation(std::string noun, const Inv &playerInventory, Inv roomObjects)
{
	if (playerInventory.searchItem(noun) >= 0)
		return player_inv;
	else if (roomObjects.searchItem(noun) >= 0)
		return room;
	else
	{
		for (int i = 0; i < roomObjects.getSize(); i++)
		{
			if (roomObjects.getItem(i)->getObjectInventory().searchItem(noun) >= 0)
				return room;
		}
	}

	return none;
}

void printNearbyItems(Player* p)
{
	std::string itemName;
	
	int numItemsInRoom = p->getPlayerCurrentRoom()->getRoomInventory().getSize();
	if (numItemsInRoom > 0)
	{
		std::cout << "From where you are in the room, you can see ";

	}

	for (int i = 0; i < numItemsInRoom; i++)
	{
		itemName = p->getPlayerCurrentRoom()->getRoomInventory().getItem(i)->getName();
		
		if (itemName.length() > 0 && itemName[itemName.length()-1] != 's')	//check if the word is plural (bug: non-plural words ending in 's' will also be considered plural)
			std::cout << "a ";
		std::cout << itemName;
		if (numItemsInRoom > 2 && i < numItemsInRoom - 2)
			std::cout << ", ";
		else if (numItemsInRoom > 2 && i == numItemsInRoom - 2)
			std::cout << ", and ";
		else if (numItemsInRoom == 2 && i == 0)
			std::cout << " and ";
		else
			std::cout << "." << std::endl;

	}
}

void printItemsInObject(GameObject* g)
{
	std::string itemName;
	
	int numItemsInObject = g->getObjectInventory().getSize();
	if (numItemsInObject > 0)
	{
		std::cout << "Near the " << g->getName() << " you see ";
	}

	for (int i = 0; i < numItemsInObject; i++)
	{
		itemName = g->getObjectInventory().getItem(i)->getName();
																											//check if the word is plural (limitation: non-plural words	
		if ( (itemName.length() > 0 && itemName[itemName.length()-1] != 's') || itemName == "Ring of Keys")	//ending in 's' will also be considered plural unless
			std::cout << "a ";																				//added as condition in if-statement)
		std::cout << itemName;
		if (numItemsInObject > 2 && i < numItemsInObject - 2)
			std::cout << ", ";
		else if (numItemsInObject > 2 && i == numItemsInObject - 2)
			std::cout << ", and ";
		else if (numItemsInObject == 2 && i == 0)
			std::cout << " and ";
		else
			std::cout << "." << std::endl;

	}
}

void loadRoomConnectionAliases(std::string targetFile, std::string direction, std::vector<std::string> &directionAliases)
{
	std::string tempString;
	std::vector<std::string> tempVector;
	if (doesFileExist((targetFile + "." + direction).c_str()) == true)	//if directional file for room exists
	{
		tempString = samsModule((targetFile + "." + direction), "NAMES");
		if (tempString != "" && tempString != "END")				//if NAMES header exists in that file
		{
			tempVector.clear();
			tempVector = stringSplitToVector(tempString);
			for (int j = 0; j < tempVector.size(); j++)
			{
				//std::cout << objectsVector[j] << std::endl; //debug print
				directionAliases.push_back(tempVector[j]);
			}
		}

	}
}

bool loadRoomLocks(std::string targetFile, std::string direction)
{
	std::string isLockedValue;
	if (doesFileExist((targetFile + "." + direction).c_str()) == true)	//if directional file for room exists
	{
		isLockedValue = samsModule((targetFile + "." + direction), "IS_LOCKED");
		if (isLockedValue == "true" || isLockedValue == "True")				
		{
			return true;
		}
	}
	return false;
}

Room* loadRoomConnections(std::string targetFile, std::string direction, const std::vector<Room*> &allRooms)
{
	std::string tempRoomString;
	Room* tempRoomPtr = NULL;

	tempRoomString = samsModule(targetFile, direction);
	if (tempRoomString == "NONE")
	{
		tempRoomPtr = NULL;
	}
	else
	{
		for (int i = 0; i < allRooms.size(); i++)
		{
			if (tempRoomString == allRooms[i]->getName())
			{
				tempRoomPtr = allRooms[i];
				break;
			}
			else if (i == allRooms.size() - 1)
				std::cout << "room link not found: " << targetFile << ":" << tempRoomString << std::endl;
		}

	}
	return tempRoomPtr;
}

void loadRoomObjects(std::string targetFile,  const std::vector<GameObject*> &allObjects, Room* &room)
{
	std::string objectsString;
	std::vector<std::string> objectsVector;

	objectsString = samsModule((targetFile), "OBJECTS");
	if (objectsString != "" && objectsString != "END")				//if OBJECTS header exists in that file and it contains items
	{
		objectsVector.clear();
		objectsVector = stringSplitToVector(objectsString);
		for (int j = 0; j < objectsVector.size(); j++)
		{
			//std::cout << objectsVector[j] << std::endl; //debug print
			for (int k = 0; k < allObjects.size(); k++)
			{
				if (objectsVector[j] == allObjects[k]->getName())
				{
					room->addItem(allObjects[k], false);
					//std::cout << objectsVector[j] << " added to " << room->getName() << std::endl; //debug print
					break;
				}
				else if (k == allObjects.size() - 1)
					std::cout << "object not found - datafile: " << targetFile << " object:" << objectsVector[j] << std::endl; //debug print
			}
			
		}
	}
}

void loadObjectObjects(std::string targetFile,  const std::vector<GameObject*> &allObjects, GameObject* &object)
{
	std::string objectsString;
	std::vector<std::string> objectsVector;

	objectsString = samsModule((targetFile), "OBJECTS");
	if (objectsString != "" && objectsString != "END")				//if OBJECTS header exists in that file and it contains items
	{
		objectsVector.clear();
		objectsVector = stringSplitToVector(objectsString);
		for (int j = 0; j < objectsVector.size(); j++)
		{
			//std::cout << objectsVector[j] << std::endl; //debug print
			for (int k = 0; k < allObjects.size(); k++)
			{
				if (objectsVector[j] == allObjects[k]->getName())
				{
					object->addItem(allObjects[k], false);
					//std::cout << objectsVector[j] << " added to " << object->getName() << std::endl; //debug print
					break;
				}
				else if (k == allObjects.size() - 1)
					std::cout << "object not found - datafile: " << targetFile << " object:" << objectsVector[j] << std::endl; //debug print
			}
			
		}
	}
}

void loadObjectAliases(std::string targetFile,  GameObject* object)
{
	std::string tempString;
	std::vector<std::string> tempVector;

	object->addAlias(stringToLowercase(object->getName()));

	tempString = samsModule((targetFile), "NAMES");
	if (tempString != "" && tempString != "END")				//if NAMES header exists in that file and it contains items
	{
		tempVector.clear();
		tempVector = stringSplitToVector(tempString);
		for (int j = 0; j < tempVector.size(); j++)
		{
			object->addAlias(tempVector[j]);
			//std::cout << objectsVector[j] << " added to " << object->getName() << std::endl;  //debug print

		}
	}
}

std::string inputToBaseName(std::string input, Inv roomInv, Inv playerInv )
{
	std::string tempString = "";
	
	//search room objects for matching input alias
	std::vector<GameObject*> invToSearch = roomInv.getInvVector();
	for (int i = 0; i < invToSearch.size(); i++)
	{
		std::vector<std::string> tempAliasVector = invToSearch[i]->getAliases();
		for (int j = 0; j < tempAliasVector.size(); j++)
		{
			if (input == tempAliasVector[j])
			{
				tempString = invToSearch[i]->getName();
				tempString = stringToLowercase(tempString);
				break;
			}

		}
		if (tempString != "")
			break;
	}

	//search player inventory objects for matching input alias
	invToSearch = playerInv.getInvVector();
	for (int i = 0; i < invToSearch.size(); i++)
	{
		std::vector<std::string> tempAliasVector = invToSearch[i]->getAliases();
		for (int j = 0; j < tempAliasVector.size(); j++)
		{
			if (input == tempAliasVector[j])
			{
				tempString = invToSearch[i]->getName();
				tempString = stringToLowercase(tempString);
				break;
			}

		}
		if (tempString != "")
			break;
	}

	//search room objects for matching input alias
	invToSearch = roomInv.getInvVector();
	for (int i = 0; i < invToSearch.size(); i++)
	{
		std::vector<GameObject*> InnerinvToSearch = invToSearch[i]->getObjectInventory().getInvVector();
		for (int k = 0; k < InnerinvToSearch.size(); k++)
		{
			std::vector<std::string> tempAliasVector = InnerinvToSearch[k]->getAliases();
				for (int j = 0; j < tempAliasVector.size(); j++)
				{
					if (input == tempAliasVector[j])
					{
						tempString = InnerinvToSearch[k]->getName();
						tempString = stringToLowercase(tempString);
						break;
					}
				}
				if (tempString != "")
				break;
		}
		if (tempString != "")
			break;
	}


	return tempString;
}

void addToInvAndRemoveFromRoom(Player &player, GameObject* selectedGameObject, GameObject* selectedGameObject2)
{
	Inv roomInv;


	if (selectedGameObject->getLootable() == true && selectedGameObject->getReachable() == true)
	{
		player.getPlayerCurrentRoom()->removeItem(selectedGameObject);	//remove item from room (if there)

		roomInv = player.getPlayerCurrentRoom()->getRoomInventory();	//remove from object in room (if there)
		for (int i = 0; i < roomInv.getSize(); i++)
		{
			if (roomInv.getItem(i)->getObjectInventory().searchItem(selectedGameObject->getName()) >= 0)
				roomInv.getItem(i)->removeItem(selectedGameObject);
		}
		player.addItem(selectedGameObject);								//add the item to player inventory if not already in their inv
	}
	if (selectedGameObject2 != NULL)
	{
		if (selectedGameObject2->getLootable() == true && selectedGameObject2->getReachable() == true)
		{
			player.getPlayerCurrentRoom()->removeItem(selectedGameObject2);	//remove second item from room (if there)
			roomInv = player.getPlayerCurrentRoom()->getRoomInventory();	//remove from object in room (if there)
			for (int i = 0; i < roomInv.getSize(); i++)
			{
				if (roomInv.getItem(i)->getObjectInventory().searchItem(selectedGameObject2->getName()) >= 0)
					roomInv.getItem(i)->removeItem(selectedGameObject2);
			}
			player.addItem(selectedGameObject2);							//add the second item to player inventory if not already in their inv				
		}
	}


}

void printHelpScreen()
{
	std::cout << "The game recognizes the following commands:\n";
	std::cout << "Look\n";
	std::cout << "Look at\n";
	std::cout << "Go \n";
	std::cout << "Take\n";
	std::cout << "Drop\n";
	std::cout << "Use\n";
	std::cout << "Combine\n";
	std::cout << "Consume\n";
	std::cout << "Ignite\n";
	std::cout << "Read\n";
	std::cout << "Wear\n";
	std::cout << "Search\n";
	std::cout << "Inventory\n";
	std::cout << "Help\n";
	std::cout << "Menu\n";
	std::cout << "Exit\n";
	std::cout << "Loadgame\n";
	std::cout << "Savegame\n";
}

void lookAtDirection(std::string selectedDirectionString, Player& player, std::string usersDirectionInput)
{
	std::string lookAtText = "";
	if (selectedDirectionString == "north")
	{
		std::string targetFile = player.getPlayerCurrentRoom()->getDataFilename() + ".FORWARD";
		if (player.getPlayerCurrentRoom()->getNorthRoom() == NULL)
			std::cout << "There are no exits to the " << usersDirectionInput << "." << std::endl;
		else if (player.getPlayerCurrentRoom()->getlockedNorth() == true)
		{
			if (doesFileExist(targetFile.c_str()) == true)
				lookAtText = samsModule(targetFile, "LOCKED");
			if (lookAtText == "END" || lookAtText == "")
				std::cout << "The " << usersDirectionInput << " exit is locked." << std::endl;
			else
				std::cout << lookAtText << std::endl;
		}
		else  //not locked
		{
			if (doesFileExist(targetFile.c_str()) == true)
				lookAtText = samsModule(targetFile, "DESCRIPTION");
			if (lookAtText == "END" || lookAtText == "")
				std::cout << "The " << usersDirectionInput << " exit is not locked." << std::endl;
			else
				std::cout << lookAtText << std::endl;
		}
	}
	else if (selectedDirectionString == "south")
	{
		std::string targetFile = player.getPlayerCurrentRoom()->getDataFilename() + ".AFT";
		if (player.getPlayerCurrentRoom()->getSouthRoom() == NULL)
			std::cout << "There are no exits to the " << usersDirectionInput << "." << std::endl;
		else if (player.getPlayerCurrentRoom()->getlockedSouth() == true)
		{
			if (doesFileExist(targetFile.c_str()) == true)
				lookAtText = samsModule(targetFile, "LOCKED");
			if (lookAtText == "END" || lookAtText == "")
				std::cout << "The " << usersDirectionInput << " exit is locked." << std::endl;
			else
				std::cout << lookAtText << std::endl;
		}
		else  //not locked
		{
			if (doesFileExist(targetFile.c_str()) == true)
				lookAtText = samsModule(targetFile, "DESCRIPTION");
			if (lookAtText == "END" || lookAtText == "")
				std::cout << "The " << usersDirectionInput << " exit is not locked." << std::endl;
			else
				std::cout << lookAtText << std::endl;
		}
	}
	else if (selectedDirectionString == "east")
	{
		std::string targetFile = player.getPlayerCurrentRoom()->getDataFilename() + ".STARBOARD";
		if (player.getPlayerCurrentRoom()->getEastRoom() == NULL)
			std::cout << "There are no exits to the " << usersDirectionInput << "." << std::endl;
		else if (player.getPlayerCurrentRoom()->getlockedEast() == true)
		{
			if (doesFileExist(targetFile.c_str()) == true)
				lookAtText = samsModule(targetFile, "LOCKED");
			if (lookAtText == "END" || lookAtText == "")
				std::cout << "The " << usersDirectionInput << " exit is locked." << std::endl;
			else
				std::cout << lookAtText << std::endl;
		}
		else  //not locked
		{
			if (doesFileExist(targetFile.c_str()) == true)
				lookAtText = samsModule(targetFile, "DESCRIPTION");
			if (lookAtText == "END" || lookAtText == "")
				std::cout << "The " << usersDirectionInput << " exit is not locked." << std::endl;
			else
				std::cout << lookAtText << std::endl;
		}
	}
	else if (selectedDirectionString == "west")
	{
		std::string targetFile = player.getPlayerCurrentRoom()->getDataFilename() + ".PORT";
		if (player.getPlayerCurrentRoom()->getWestRoom() == NULL)
			std::cout << "There are no exits to the " << usersDirectionInput << "." << std::endl;
		else if (player.getPlayerCurrentRoom()->getlockedWest() == true)
		{
			if (doesFileExist(targetFile.c_str()) == true)
				lookAtText = samsModule(targetFile, "LOCKED");
			if (lookAtText == "END" || lookAtText == "")
				std::cout << "The " << usersDirectionInput << " exit is locked." << std::endl;
			else
				std::cout << lookAtText << std::endl;
		}
		else  //not locked
		{
			if (doesFileExist(targetFile.c_str()) == true)
				lookAtText = samsModule(targetFile, "DESCRIPTION");
			if (lookAtText == "END" || lookAtText == "")
				std::cout << "The " << usersDirectionInput << " exit is not locked." << std::endl;
			else
				std::cout << lookAtText << std::endl;
		}
	}

}

void loadPlayerInventory(std::string targetFile,  const std::vector<GameObject*> &allObjects, Player& player)
{
	std::string objectsString;
	std::vector<std::string> objectsVector;

	objectsString = samsModule((targetFile), "OBJECTS");
	if (objectsString != "" && objectsString != "END")				//if OBJECTS header exists in that file and it contains items
	{
		objectsVector.clear();
		objectsVector = stringSplitToVector(objectsString);
		for (int j = 0; j < objectsVector.size(); j++)
		{
			//std::cout << objectsVector[j] << std::endl; //debug print
			for (int k = 0; k < allObjects.size(); k++)
			{
				if (objectsVector[j] == allObjects[k]->getName())
				{
					player.addItem(allObjects[k], false);
					//std::cout << objectsVector[j] << " added to player inventory" << std::endl; //debug print
					break;
				}
				else if (k == allObjects.size() - 1)
					std::cout << "playerinvload: object not found - datafile: " << targetFile << " object:" << objectsVector[j] << std::endl; //debug print
			}
			
		}
	}
}

void loadPodMissingParts(std::string targetFile,  const std::vector<GameObject*> &allObjects, EscapePod &object)
{
	std::string objectsString;
	std::vector<std::string> objectsVector;

	objectsString = samsModule((targetFile), "MISSINGPARTS");
	if (objectsString != "" && objectsString != "END")				//if MISSINGPARTS header exists in that file and it contains items
	{
		objectsVector.clear();
		objectsVector = stringSplitToVector(objectsString);
		for (int j = 0; j < objectsVector.size(); j++)
		{
			//std::cout << objectsVector[j] << std::endl; //debug print
			for (int k = 0; k < allObjects.size(); k++)
			{
				if (objectsVector[j] == allObjects[k]->getName())
				{
					object.addMissingPart(allObjects[k]);
					//std::cout << objectsVector[j] << " added to " << object.getName() << std::endl; //debug print
					break;
				}
				else if (k == allObjects.size() - 1)
					std::cout << "object not found - datafile: " << targetFile << " object:" << objectsVector[j] << std::endl; //debug print
			}
			
		}
	}
}