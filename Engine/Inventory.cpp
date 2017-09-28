#include "Inventory.h"
#include "GameObject.h"

/*************************************
Inventory::getSize()
Desc:	Returns the number of items in the player's inventory
**************************************/
int Inv::getSize()
{
	return playerInv.size();
}


/*************************************
Inventory::searchItem(int item)
Desc:	Searches the player's inventory for an item. Returns the index of item of -1 if not found.
Param:	int item - the enum Item value of the item to be searched for
**************************************/
int Inv::searchItem(std::string item) const
{
	std::string tempString;
	//Check if item is in the player's inventory. If it is, return the index. If it's not, return -1.
	for (int i = 0; i < playerInv.size(); i++)
	{
		tempString = playerInv[i]->getName();
		if (item == tempString || item == stringToLowercase(tempString))
		{
			return i;
		}
	}
	return -1;
}


/*************************************
Inventory::addItem(int item)
Desc:	Adds an item to the player's inventory (if the item is not already in their inventory)
Param:	int item - the enum Item value of the item to be added
**************************************/
int Inv::addItem(GameObject* item, bool writeToDatafile)
{
	//Check if item is already in inventory. If it is, print "already have" msg and return -1.
	if (searchItem(item->getName()) >= 0) 
	{
		//std::cout << "You already have a " << item->getName() << " in your inventory." << std::endl;
		return -1;
	}
	//If item is not in inventory, print "added item" msg and return 0.
	playerInv.push_back(item);
	//std::cout << item->getName() << " added to inventory." << std::endl;
	if (writeToDatafile && dataFilename != "")
		dropObject(dataFilename, item->getName(), "OBJECTS");
	return 0;
}


/*************************************
Inventory::removeItem(int item)
Desc:	Adds an item to the player's inventory (if the item is not already in their inventory)
Param:	int item - the enum Item value of the item to be removed
**************************************/
int Inv::removeItem(GameObject* item)
{
	//Check if item is in inventory. If it is, print "dropped item" msg and return 0.
	if (searchItem(item->getName()) >= 0)
	{
			playerInv.erase(playerInv.begin() + searchItem(item->getName()));
			//std::cout << "You dropped " << item->getName() << "." << std::endl;
			if (item->getLootable() == true && dataFilename != "")
				pickUpObject(dataFilename, item->getName(), "OBJECTS");
			return 0;
		}
	//If item is not in inventory, print "don't have item" msg and return -1.
	//std::cout << "You don't have a " << item->getName() << " in your inventory." << std::endl;
	return -1;
}


/*************************************
Inventory::printInventory()
Desc:	Prints player's inventory in numbered list format 
		(items are listed in the order they were added to inventory)
**************************************/
void Inv::printInventory()
{
	std::cout << "Player Inventory:" << std::endl;
	if (playerInv.size() == 0)
		std::cout << "<empty>" << std::endl;
	else
		for (int i = 0; i < playerInv.size(); i++)
			std::cout << i + 1 << ". " << playerInv[i]->getName() << std::endl;
}

/*
int main()
{
	//basic testing
	Inventory inv;

	std::cout << "# of items in inventory: " << inv.getSize() << std::endl;

	inv.addItem(multitool);
	inv.addItem(crowbar);
	inv.addItem(crowbar);
	inv.addItem(medkit);

	inv.printInventory();

	inv.removeItem(crowbar);

	inv.printInventory();
	std::cout << "# of items in inventory: " << inv.getSize() << std::endl;

	inv.removeItem(crowbar);
	inv.addItem(crowbar);
	inv.printInventory();

	std::cout << "# of items in inventory: " << inv.getSize() << std::endl;
	
	std::cin.get();

	return 0;
}
*/

GameObject* Inv::getItem(int i)
{
	if (i >= 0 && i < playerInv.size())
		return playerInv[i];
	else
		return NULL;
}

std::vector<GameObject*> Inv::getInvVector()
{
	return playerInv;
}

void Inv::clearInventory()
{
	playerInv.clear();
}

void Inv::setDataFilename(std::string s)
{
	dataFilename = s;
}