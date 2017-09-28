#include "GameManager.h"
#include <iostream>
#include "helperFunctions.h"
#include "Game.h"
#include "fileops.h"

//prototypes
void startNewGame(menuSelection& menuChoice);
void loadSavedGame(menuSelection& menuChoice);

void GameManager::initializeGame()
{
	do
	{
		if (menuChoice == fastLoad)
			menuChoice = loadSaveGame;
		else
		{
			printMainMenu();
			menuChoice = getMainMenuInput();
		}

		switch (menuChoice)
		{
			case newGame:
				startNewGame(menuChoice);
				break;
			case loadSaveGame:
				loadSavedGame(menuChoice);
				break;
			case help:
				printHelpScreen();
				break;
			case exitGame:
				std::cout << "Thanks for playing. Exiting Game.\n";
				break;
			default:
				break;
		}
	} while (menuChoice != exitGame);
}

void GameManager::printMainMenu()
{
	std::cout << std::string( 100, '\n' );
	
	centerstring(" ______     ______     ______     ______     ______   ______          \n");
	centerstring("/\\  ___\\   /\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\ /\\  ___\\         \n");
	centerstring("\\ \\  __\\   \\ \\___  \\  \\ \\ \\____  \\ \\  __ \\  \\ \\  _-/ \\ \\  __\\         \n");
	centerstring(" \\ \\_____\\  \\/\\_____\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_\\    \\ \\_____\\       \n");
	centerstring("  \\/_____/   \\/_____/   \\/_____/   \\/_/\\/_/   \\/_/     \\/_____/       \n");
	centerstring(" ______   ______     __  __     ______     __  __     ______          \n");
	centerstring("/\\__  _\\ /\\  __ \\   /\\ \\/\\ \\   /\\  == \\   /\\ \\/\\ \\   /\\  ___\\         \n");
	centerstring("\\/_/\\ \\/ \\ \\  __ \\  \\ \\ \\_\\ \\  \\ \\  __<   \\ \\ \\_\\ \\  \\ \\___  \\        \n");
	centerstring("   \\ \\_\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\/\\_____\\       \n");
	centerstring("    \\/_/   \\/_/\\/_/   \\/_____/   \\/_/ /_/   \\/_____/   \\/_____/       \n");
	centerstring(" ______     ______   ______     ______   __     ______     __   __    \n");
	centerstring("/\\  ___\\   /\\__  _\\ /\\  __ \\   /\\__  _\\ /\\ \\   /\\  __ \\   /\\ \"-.\\ \\   \n");
	centerstring("\\ \\___  \\  \\/_/\\ \\/ \\ \\  __ \\  \\/_/\\ \\/ \\ \\ \\  \\ \\ \\/\\ \\  \\ \\ \\-.  \\  \n");
	centerstring(" \\/\\_____\\    \\ \\_\\  \\ \\_\\ \\_\\    \\ \\_\\  \\ \\_\\  \\ \\_____\\  \\ \\_\\\\\"\\_\\ \n");
	centerstring("   \\/_____/     \\/_/   \\/_/\\/_/     \\/_/   \\/_/   \\/_____/   \\/_/ \\/_/ \n\n");

	centerstring("Main Menu\n\n");
	centerstring("1. New Game \n");
    centerstring("2. Load Game \n");                                                                
	centerstring("3. Help     \n");  
	centerstring("4. Exit     \n\n");  

	centerstring("Enter a number from the Main Menu list: ");

}

menuSelection GameManager::getMainMenuInput()
{
	std::string userInput;
	std::getline(std::cin, userInput);
	
	if (userInput == "1")
		return newGame;
	else if (userInput == "2")
		return loadSaveGame;
	else if (userInput == "3")
		return help;
	else if (userInput == "4")
		return exitGame;
	else
		return invalidChoice;
}

void GameManager::printHelpScreen()
{
	std::cout << std::string(100, '\n');
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
	std::cout << "Savegame\n\n";
	std::cout << "Press Enter to return to the Main Menu...";
	//std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
	std::cin.get();

}


void startNewGame(menuSelection& menuChoice)
{
	int gameNum = saveGame("");			//copy original datafiles to new folder
	
	Game* newGame = new Game(gameNum);
	newGame->loadGameFiles(to_string(gameNum));	//load game from those newly copied files
	std::cout << std::string(100, '\n');
	int gameExitCode = newGame->playGame();
	delete newGame;
	if (gameExitCode == hardExit)
		menuChoice = exitGame;
	else if (gameExitCode == quickLoad)
		menuChoice = fastLoad;

}

void loadSavedGame(menuSelection& menuChoice)
{
	std::string gameToLoad;

	std::cout << std::string(100, '\n');
	gameToLoad = loadGame();
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::string saveGameSearch = samsModule("../sg_savedgameslist", gameToLoad);
	if (saveGameSearch == "END" || saveGameSearch == "")
	{
		std::cout << "Invalid game file number. Press Enter to return to Main Menu...";
		cin.get();
	}
	else
	{
		int gameNum = saveGame(gameToLoad);					//copy saved game files into new folder
		Game* newLoadedGame = new Game(gameNum);	
		newLoadedGame->loadGameFiles(to_string(gameNum));	//load game from those newly copied files
		std::cout << std::string(100, '\n');
		int gameExitCode = newLoadedGame->playGame();
		delete newLoadedGame;
		if (gameExitCode == hardExit)
			menuChoice = exitGame;
		else if (gameExitCode == quickLoad)
			menuChoice = fastLoad;
	}
}
