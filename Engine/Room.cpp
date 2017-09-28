#include "Room.h"
#include "Player.h"

void Room::setName(std::string name)
{
	this->name = name;
}

void Room::setLongDescription(std::string desc)
{
	longDescription = desc;
}

void Room::setShortDescription(std::string desc)
{
	shortDescription = desc;
}

void Room::setPlayerHasBeenHereBefore(bool b)
{
	playerHasBeenHereBefore = b;
}

void Room::setDataFilename(std::string s)
{
	dataFilename = s;
	roomObjects.setDataFilename(s);
}

std::string Room::getName()
{
	return name;
}

std::string Room::getLongDescription()
{
	return longDescription;
}

std::string Room::getShortDescription()
{
	return shortDescription;

}

bool Room::getPlayerHasBeenHereBefore()
{
	return playerHasBeenHereBefore;
}

std::string Room::getDataFilename()
{
	return dataFilename;
}

Inv Room::getRoomInventory()
{
	return roomObjects;
}

int Room::addItem(GameObject* go, bool writeToDatafile)
{
	return roomObjects.addItem(go, writeToDatafile);
}

int Room::removeItem(GameObject* go)	
{
	return roomObjects.removeItem(go);
}

bool Room::canSeeObjects(Player* p)
{
	return true;
}


bool Room::goDirection(std::string direction, Player* p)
{
	std::string lockDescription;
	std::string doorName = "";
	std::string playerCurrentRoomName = p->getPlayerCurrentRoom()->getName();
	
	if (playerCurrentRoomName == "Engineering Bay")
	{
		if (direction == "north")
			doorName = "Decon Station Door";
		else if (direction == "south")
			doorName = "Security Gate";
	}
	else if (playerCurrentRoomName == "Security Office")
	{
		if (direction == "east")
			doorName = "Armory Door";
	}

	if (direction == "north")
	{
		if (lockedNorth == true)
		{
			if (doorName != "")
				std::cout << "The " << doorName << " is locked." << std::endl;
			else
				std::cout << "The Forward door is locked." << std::endl;
			return false;
		}
		else if (North == NULL)
		{
			std::cout << "There is no Forward exit." << std::endl;
			return false;
		}
		else
		{
			if (doorName != "")
				std::cout << "Exited " << this->name << " through the forward " << doorName << ".\n" << std::endl;
			else
				std::cout << "Exited " << this->name << " through Forward exit.\n" << std::endl;
			return true;
		}
	}
	if (direction == "south")
	{
		if (lockedSouth == true)
		{
			if (name == "Recreation Room")
				std::cout << "The Aft passage is collapsed and blocked by debris." << std::endl;
			else if (doorName != "")
				std::cout << "The " << doorName << " is locked." << std::endl;
			else
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
			if (doorName != "")
				std::cout << "Exited " << this->name << " through the aft " << doorName << ".\n" << std::endl;
			else
				std::cout << "Exited " << this->name << " through Aft exit.\n" << std::endl;
			return true;
		}
	}
	if (direction == "west")
	{
		if (lockedWest == true)
		{
			if (doorName != "")
				std::cout << "The " << doorName << " is locked." << std::endl;
			else
				std::cout << "The Port door is locked." << std::endl;		
			return false;
		}
		else if (West == NULL)
		{
			std::cout << "There is no Port exit." << std::endl;
			return false;
		}
		else
		{
			if (doorName != "")
				std::cout << "Exited " << this->name << " through the port " << doorName << ".\n" << std::endl;
			else
				std::cout << "Exited " << this->name << " through Port exit.\n" << std::endl;
			return true;
		}
	}
	if (direction == "east")
	{
		if (lockedEast == true)
		{
			if (doorName != "")
				std::cout << "The " << doorName << " is locked." << std::endl;
			else
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
			if (doorName != "")
				std::cout << "Exited " << this->name << " through the starboard " << doorName << ".\n" << std::endl;
			else
				std::cout << "Exited " << this->name << " through Starboard exit.\n" << std::endl;
			return true;
		}
	}
}



std::string Room::look(Player* p)
{
	return longDescription;
}