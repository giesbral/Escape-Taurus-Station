#ifndef INVENTORY_H
#define INVENTORY_H

//notes: currently no max capacity on inventory, though this could be added if desired (say, player can hold a max of 10 items at a time)

#include <vector>
#include <string>
#include <iostream>

#include "helperFunctions.h"
#include "fileparse.h"

/*************************************
Inventory class
Desc:	Represents the player's inventory. Methods allow for adding, removing, and searching for items in
		the player's inventory. Additional functions enable printing the items in invetory and getting its size.
**************************************/
class GameObject;

class Inv
{
private:
	std::vector<GameObject*> playerInv;	//vector holding int values corresponding to enum Item values of items
	std::string dataFilename;
public:
	int getSize();			//returns number of items in inventory
	int searchItem(std::string) const;	//return index of item in inventory (or -1 if not found)
	int addItem(GameObject*, bool writeToDataFile = true);		//adds an item to inventory
	int removeItem(GameObject*);	//removes an item from inventory
	std::vector<GameObject*> getInvVector();
	void printInventory();	//prints list of items in inventory
	GameObject* getItem(int);	//gets item at index 
	void clearInventory();
	void setDataFilename(std::string);
};


#endif