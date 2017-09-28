#include "CaptainsQuarters.h"

CaptainsQuarters::CaptainsQuarters()
{

}



bool CaptainsQuarters::canSeeObjects(Player* p)
{
	GameObject* tempGoPtr = p->getPlayerInventory().getItem(p->getPlayerInventory().searchItem("flashlight"));
	
	if (tempGoPtr != NULL)
		if (tempGoPtr->getActivated() == true)
			return true;
	
	return false;

}

std::string CaptainsQuarters::look(Player* p)
{
	if (canSeeObjects(p) == true)
		return longDescription;
	else
		return samsModule("../CAPTQUARTERS", "DARK");


	
}