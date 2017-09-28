#include "Flashlight.h"
#include "Player.h"


Flashlight::Flashlight()
{
	setName("flashlight");
	setLongDescription("It's a flashlight (placeholder long desc).");
	setLootable(true);
	setReady(false);
}

void Flashlight::use(GameObject* go, Player* player)
{
	player->addItem(this);		//add the item(s) to player inventory if not already in their inv
	if (go != NULL)
		player->addItem(go);

	if (go == NULL)
	{
		if (inv.searchItem("battery") >= 0)
		{
			if (getReady() == false)
			{
				setReady(true);
				std::cout << "You turned the flashlight on." << std::endl;
			}
			else
			{
				setReady(false);
				std::cout << "You turned the flashlight off." << std::endl;
			}
		}
		else
		{
			std::cout << "It doesn't turn on. It's missing batteries." << std::endl;
		}
	}
	else if (go->getName() == "battery")
	{
		inv.addItem(go);	//add battery to flashlight inv
		player->getPlayerCurrentRoom()->removeItem(go);	//remove battery from room (if it was there)
		player->removeItem(go);	//remove battery from player's inventory (if it was there)
		std::cout << "You put the batteries in the flashlight." << std::endl;
	}
	else
	{
		std::cout << "You can't use " << go->getName() << " with the flashlight." << std::endl;
	}
}

void Flashlight::consume(GameObject* go)
{
	std::cout << "You got it in your mouth but the flashlight was too hard to chew." << std::endl;
}