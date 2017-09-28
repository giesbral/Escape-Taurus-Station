#include "EscapePod.h"
#include "fileparse.h"
#include "Player.h"

void EscapePod::removeMissingPart(GameObject* g)
{
	missingParts.removeItem(g);
	pickUpObject(dataFilename, g->getName(), "MISSINGPARTS");
}

void EscapePod::addMissingPart(GameObject* g)
{
	missingParts.addItem(g);
}

bool EscapePod::isFullyRepaired()
{
	if (missingParts.getSize() == 0)
		return true;
	
	return false;

}

void EscapePod::printMissingParts()
{
	if (this->isFullyRepaired() == false)
	{
		std::cout << "Parts requiring replacement: ";
		for (int i = 0; i < missingParts.getSize(); i++)
		{
			std::cout << missingParts.getItem(i)->getName();
			if (i < missingParts.getSize() - 1)
				std::cout << ", ";
			else
				std::cout << "." << std::endl;
		}
	}
}

void EscapePod::combine(GameObject* go2, Player* p)
{
	std::string combineText;
	
	if (ready == false)
	{
		if (go2 == NULL)
		{
			combineText = samsModule(dataFilename, "COMBINE");
			if (combineText != "END" && combineText != "")
				std::cout << combineText << std::endl;
			else
				std::cout << "You must specify two items to combine." << std::endl;
		}
		else
		{
			combineText = samsModule(dataFilename, "COMBINE***" + go2->getHeaderName());
			if (combineText != "END" && combineText != "")
			{
				std::cout << combineText << std::endl;
				p->removeItem(go2);
				removeMissingPart(go2);
				if (isFullyRepaired() == true)
					setReady(true);
			}
			else
				std::cout << "The " << go2->getName() << " can't be combined with the " << name << std::endl;
		}
	}
	else  //ready == true
	{
		if (go2 == NULL)
		{
			combineText = samsModule(dataFilename, "COMBINE***READY");
			if (combineText != "END" && combineText != "")
			{
				std::cout << combineText << std::endl;
				setActivated(true);
			}
			else
				std::cout << "The " << name << " can't be combined with any more items." << std::endl;
		}
		else
			std::cout << "The " << go2->getName() << " can't be combined with the " << name << std::endl;
	}
}

void EscapePod::use(GameObject* go2, Player* p)
{
	std::string useText;
	
	if (ready == false)
	{
		if (go2 == NULL)
		{
			useText = samsModule(dataFilename, "USE");
			if (useText != "END" && useText != "")
				std::cout << useText << std::endl;
			else
				std::cout << "The " << name << " can't be used." << std::endl;
			printMissingParts();
		}
		else
		{
			useText = samsModule(dataFilename, "USE***" + go2->getHeaderName());
			if (useText != "END" && useText != "")
			{
				std::cout << useText << std::endl;
				p->removeItem(go2);
				removeMissingPart(go2);
				if (isFullyRepaired() == true)
					setReady(true);
			}
			else
				std::cout << "The " << go2->getName() << " can't be used with the " << name << std::endl;
		}
	}
	else  //ready == true
	{
		if (go2 == NULL)
		{
			if (p->getPlayerInventory().searchItem("Security Footage") >= 0)
				useText = samsModule(dataFilename, "USE***ACTIVATED");
			else
				useText = samsModule(dataFilename, "USE***READY");
			if (useText != "END" && useText != "")
			{
				std::cout << useText << std::endl;
				activated = true;												//might be special case (as in flashlight and battery)
			}
			else
				std::cout << "The " << name << " can't be used." << std::endl;
		}
		else
			std::cout << "The " << go2->getName() << " can't be used with the " << name << std::endl;

	}
}