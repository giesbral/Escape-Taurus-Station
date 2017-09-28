#include "Solarium.h"

Solarium::Solarium()
{

}



bool Solarium::canSeeObjects(Player* p)
{
	GameObject* tempGoPtr = p->getPlayerInventory().getItem(p->getPlayerInventory().searchItem("flashlight"));
	
	if (tempGoPtr != NULL)
		if (tempGoPtr->getActivated() == true)
			return true;
	
	return false;

}

std::string Solarium::look(Player* p)
{
	if (canSeeObjects(p) == true)
		return longDescription;
	else
		return samsModule("../SOLARIUM", "DARK");


	
}


bool Solarium::goDirection(std::string direction, Player* p)
{
	std::string lockDescription;

	if (direction == "north")
	{
		if (lockedNorth == true)
		{
			std::cout << "The forward blast door is locked." << std::endl;
			return false;
		}
		else if (North == NULL)
		{
			std::cout << "There is no Forward exit." << std::endl;
			return false;
		}
		else
		{
			std::cout << "Exited " << this->name << " through Forward exit.\n" << std::endl;
			return true;
		}
	}
	if (direction == "south")
	{
		if (lockedSouth == true)
		{
			std::cout << "The Aft door is locked." << std::endl;
			return false;
		}
		else if (South == NULL)
		{
			std::cout << "There is no Aft exit." << std::endl;
			return false;
		}
		else
		{
			std::cout << "Exited " << this->name << " through Aft exit.\n" << std::endl;
			return true;
		}
	}
	if (direction == "west")
	{
		if (lockedWest == true)
		{
			std::cout << "The airlock is locked." << std::endl;		
			return false;
		}
		else if (West == NULL)
		{
			std::cout << "There is no Port exit." << std::endl;
			return false;
		}
		else
		{
			GameObject* spacesuit = NULL;
			bool spacesuitBeingWorn = false;
			Inv playerInv = p->getPlayerInventory();
			int spacesuitIndex = playerInv.searchItem("Spacesuit");
			if (spacesuitIndex >= 0)
				spacesuit = playerInv.getItem(spacesuitIndex);
			if (spacesuit != NULL)
				spacesuitBeingWorn = spacesuit->getBeingWornByPlayer();
			
			if (spacesuitBeingWorn == true)
			{
				std::cout << "Exited " << this->name << " through the airlock.\n" << std::endl;
				return true;
			}
			else
				std::cout << "You think better of entering the airlock without a spacesuit on." << std::endl;
			
			return false;
		}
	}
	if (direction == "east")
	{
		if (lockedEast == true)
		{
			std::cout << "The Starboard door is locked." << std::endl;
			return false;
		}
		else if (East == NULL)
		{
			std::cout << "There is no Starboard exit." << std::endl;
			return false;
		}
		else
		{
			std::cout << "Exited " << this->name << " through Starboard exit.\n" << std::endl;
			return true;
		}
	}
}