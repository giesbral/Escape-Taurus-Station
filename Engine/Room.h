#ifndef ROOM_H
#define ROOM_H


#include <string>
#include "Inventory.h"
#include "Flashlight.h"
#include "GameObject.h"
#include "fileparse.h"
#include "globals.h"
#include "helperFunctions.h"

class Player;

class Room
{

friend class Game;	//for access to roomObjects

protected:
	std::string name;
	std::string longDescription;
	std::string shortDescription;
	std::string dataFilename;
	bool playerHasBeenHereBefore;
	Inv roomObjects;
		
	Room* North;
	Room* South;
	Room* East;
	Room* West;

	//Basic direction aliases. Additional room-specific aliases from ROOMNAME.DIRECTION data files will
	//be added to these lists on a room by room basis.
	std::vector<std::string> northAliases = { "north", "fore", "forward", "f", "n" };
	std::vector<std::string> southAliases = { "south", "aft", "a", "s" };
	std::vector<std::string> eastAliases = { "east", "starboard", "e"};	//did not include "s" here due to conflict with south's "s"
	std::vector<std::string> westAliases = { "west", "port", "p", "w" };

	bool lockedNorth = false;
	bool lockedSouth = false;
	bool lockedEast = false;
	bool lockedWest = false;

public:
	void setName(std::string);
	void setLongDescription(std::string);
	void setShortDescription(std::string);
	void setPlayerHasBeenHereBefore(bool);
	void setDataFilename(std::string);

	std::string getName();
	std::string getLongDescription();
	std::string getShortDescription();
	bool getPlayerHasBeenHereBefore();
	std::string getDataFilename();
	Inv getRoomInventory();

	bool getlockedNorth() { return lockedNorth; }
	bool getlockedSouth() { return lockedSouth; }
	bool getlockedEast() { return lockedEast; }
	bool getlockedWest() { return lockedWest; }

	void setlockedNorth(bool b) {  lockedNorth = b; }
	void setlockedSouth(bool b) {  lockedSouth = b; }
	void setlockedEast(bool b) {  lockedEast = b; }
	void setlockedWest(bool b) {  lockedWest = b; }

	Room* getNorthRoom() { return North; }
	Room* getSouthRoom() { return South; }
	Room* getEastRoom() { return East; }
	Room* getWestRoom() { return West; }

	int addItem(GameObject*, bool writeToDatafile = true);		
	int removeItem(GameObject*);	

	virtual bool canSeeObjects(Player* p);
	virtual bool goDirection(std::string, Player* p);
	virtual std::string look(Player* p);



};




#endif