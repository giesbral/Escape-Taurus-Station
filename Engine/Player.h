#ifndef PLAYER_H
#define PLAYER_H

#include "Room.h"
#include "Inventory.h"
#include "GameObject.h"

class Player
{

protected:
	Room* playerCurrentRoom;
	Room* playerPreviousRoom;

	Inv playerInventory;

public:
	void setPlayerCurrentRoom(Room*);
	void setPlayerPreviousRoom(Room*);
	void setInventoryDatafileName(std::string);

	Room* getPlayerCurrentRoom();
	Room* getPlayerPreviousRoom();

	Inv getPlayerInventory();

	int addItem(GameObject*, bool writeToDatafile = true);		
	int removeItem(GameObject*);	

	void clearPlayerInventory();

};

#endif