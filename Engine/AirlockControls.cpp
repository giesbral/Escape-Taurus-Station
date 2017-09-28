#include "AirlockControls.h"
#include "fileparse.h"
#include "Player.h"

void AirlockControls::use(GameObject* go2, Player* p)
{
	std::string useText;

	GameObject* spacesuit = NULL;
	bool spacesuitBeingWorn = false;
	Inv playerInv = p->getPlayerInventory();
	int spacesuitIndex = playerInv.searchItem("Spacesuit");
	if (spacesuitIndex >= 0)
		spacesuit = playerInv.getItem(spacesuitIndex);
	if (spacesuit != NULL)
		spacesuitBeingWorn = spacesuit->getBeingWornByPlayer();
	
	if (spacesuitBeingWorn == true)
		this->setReady(true);
	else
		this->setReady(false);

	if (ready == false)
	{
		if (go2 == NULL)
		{
			useText = samsModule(dataFilename, "USE");
			if (useText != "END" && useText != "")
				std::cout << useText << std::endl;
			else
				std::cout << "The " << name << " can't be used." << std::endl;
		}
		else
		{
			useText = samsModule(dataFilename, "USE***" + go2->getHeaderName());
			if (useText != "END" && useText != "")
			{
				std::cout << useText << std::endl;
			}
			else
				std::cout << "The " << go2->getName() << " can't be used with the " << name << std::endl;
		}
	}
	else  //ready == true
	{
		if (go2 == NULL)
		{
			useText = samsModule(dataFilename, "USE***READY");
			if (useText != "END" && useText != "")
			{
				std::cout << useText << std::endl;
				p->getPlayerCurrentRoom()->setlockedWest(false); //unlock the airlock
				std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".PORT";
				pickUpObject(directionFile, "true", "IS_LOCKED");
				pickUpObject(directionFile, "false", "IS_LOCKED");
				dropObject(directionFile, "false", "IS_LOCKED");	//write unlock to datafile

			}
			else
				std::cout << "The " << name << " can't be used." << std::endl;
		}
		else
			std::cout << "The " << go2->getName() << " can't be used with the " << name << std::endl;

	}
}