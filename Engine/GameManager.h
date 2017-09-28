#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <string>

enum menuSelection {invalidChoice, newGame, loadSaveGame, help, exitGame, fastLoad};

class GameManager
{
protected:
	std::string userMenuInput;
	menuSelection menuChoice;

public:
	void initializeGame();
	void printMainMenu();
	menuSelection getMainMenuInput();
	void printHelpScreen();



};




#endif