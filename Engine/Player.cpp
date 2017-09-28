#include "Player.h"

void Player::setPlayerCurrentRoom(Room* r)
{
	playerCurrentRoom = r;
}
void Player::setPlayerPreviousRoom(Room* r)
{
	playerPreviousRoom = r;
}

void Player::setInventoryDatafileName(std::string s)
{
	playerInventory.setDataFilename(s);
}

Room* Player::getPlayerCurrentRoom()
{
	return playerCurrentRoom;
}

Room* Player::getPlayerPreviousRoom()
{
	return playerPreviousRoom;
}

Inv Player::getPlayerInventory()
{
	return playerInventory;
}

int Player::addItem(GameObject* go, bool writeToDatafile)
{
	return playerInventory.addItem(go, writeToDatafile);
}

int Player::removeItem(GameObject* go)	
{
	return playerInventory.removeItem(go);
}

void Player::clearPlayerInventory()
{
	playerInventory.clearInventory();
}