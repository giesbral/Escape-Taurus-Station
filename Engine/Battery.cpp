#include "Battery.h"
#include "Player.h"

Battery::Battery()
{
	setName("battery");
	setLongDescription("It's a Battery (placeholder long desc).");
	setLootable(true);
	setReady(false);
}

void Battery::use(GameObject* go, Player* player)
{
	player->addItem(this);		//add the item(s) to player inventory if not already in their inv
	if (go != NULL)
		player->addItem(go);

	if (go == NULL)
	{
		std::cout << "You can't do anything with the battery on it's own. Maybe you can use it with something that takes batteries." << std::endl;
	}
	else if (go->getName() == "flashlight")
	{
		go->use(this, player);
		
		//inv.addItem(go);	//add battery to flashlight inv
		//player->getPlayerCurrentRoom()->removeItem(go);	//remove battery from room (if it was there)
		//player->removeItem(go);	//remove battery from player's inventory (if it was there)
		//std::cout << "You put the batteries in the flashlight." << std::endl;
	}
	else
	{
		std::cout << "You can't use " << go->getName() << " with the flashlight." << std::endl;
	}
}


void Battery::consume(GameObject* go)
{
	std::cout << "It gave you a little shock when it touched your tongue but you couldn't eat it." << std::endl;
}