#include "Armory.h"

Armory::Armory()
{

}



bool Armory::canSeeObjects(Player* p)
{
	GameObject* tempGoPtr = p->getPlayerInventory().getItem(p->getPlayerInventory().searchItem("flashlight"));
	
	if (tempGoPtr != NULL)
		if (tempGoPtr->getActivated() == true)
			return true;
	
	return false;

}

std::string Armory::look(Player* p)
{
	if (canSeeObjects(p) == true)
		return longDescription;
	else
		return samsModule("../ARMORY", "DARK");


	
}