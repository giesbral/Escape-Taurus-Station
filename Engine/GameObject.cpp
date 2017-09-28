#include "GameObject.h"
#include "Player.h"
#include "fileparse.h"

void GameObject::setName(std::string name)
{
	this->name = name;
}

void GameObject::setLongDescription(std::string desc)
{
	longDescription = desc;
}

void GameObject::setLootable(bool b)
{
	lootable = b;
}

void GameObject::setReady(bool b)
{
	ready = b;
}

void GameObject::setActivated(bool b)
{
	activated = b;
}

void GameObject::setUseableWith(std::string s)
{
	useableWith = s;
}

void GameObject::setDataFilename(std::string s)
{
	dataFilename = s;
	objectObjects.setDataFilename(s);
}

void GameObject::setHeaderName(std::string s)
{
	headerName = s;
}

void GameObject::setSearched(bool b)
{
	beenSearched = b;
}

void GameObject::setBeingWornByPlayer(bool b)
{
	beingWornByPlayer = b;
}

void GameObject::addAlias(std::string s)
{
	aliases.push_back(s);
}

void GameObject::setReachable(bool b)
{
	reachable = b;
}

bool GameObject::getReachable()
{
	return reachable;
}

std::string GameObject::getName()
{
	return name;
}

std::string GameObject::getLongDescription()
{
	return longDescription;
}

bool GameObject::getLootable()
{
	return lootable;
}

bool GameObject::getReady()
{
	return ready;
}

bool GameObject::getActivated()
{
	return activated;
}

std::string GameObject::getDataFilename()
{
	return dataFilename;
}

std::string GameObject::getHeaderName()
{
	return headerName;
}

std::vector<std::string> GameObject::getAliases()
{
	return aliases;
}

Inv GameObject::getObjectInventory()
{
	return objectObjects;
}

int GameObject::addItem(GameObject* go,  bool writeToDatafile)
{
	return objectObjects.addItem(go, writeToDatafile);
}

int GameObject::removeItem(GameObject* go)	
{
	return objectObjects.removeItem(go);
}

bool GameObject::getSearched()	
{
	return beenSearched;
}

bool GameObject::getBeingWornByPlayer()	
{
	return beingWornByPlayer;
}

void GameObject::lookAt(GameObject* go)
{
	std::string lookAtText;

	if (ready == false)
	{
		if (activated == true)
		{
			lookAtText = samsModule(dataFilename, "LOOK_AT***ACTIVATED");
			if (lookAtText != "END" && lookAtText != "")
				std::cout << lookAtText << std::endl;
			else
			{
				lookAtText = samsModule(dataFilename, "LOOK_AT");
				if (lookAtText != "END" && lookAtText != "")
					std::cout << lookAtText << std::endl;
				else
					std::cout << "It's just a " << name << "." << std::endl;
			}
		}
		else
		{
			lookAtText = samsModule(dataFilename, "LOOK_AT");
			if (lookAtText != "END" && lookAtText != "")
				std::cout << lookAtText << std::endl;
			else
				std::cout << "It's just a " << name << "." << std::endl;
		}
	}
	else
	{
		if (activated == true)
		{
			lookAtText = samsModule(dataFilename, "LOOK_AT***ACTIVATED");
			if (lookAtText != "END" && lookAtText != "")
				std::cout << lookAtText << std::endl;
			else
			{
				lookAtText = samsModule(dataFilename, "LOOK_AT***READY");
				if (lookAtText != "END" && lookAtText != "")
					std::cout << lookAtText << std::endl;
				else
				{
					lookAtText = samsModule(dataFilename, "LOOK_AT");
					if (lookAtText != "END" && lookAtText != "")
						std::cout << lookAtText << std::endl;
					else
						std::cout << "It's just a " << name << "." << std::endl;
				}
			}
		}
		else
		{
			lookAtText = samsModule(dataFilename, "LOOK_AT***READY");
			if (lookAtText != "END" && lookAtText != "")
				std::cout << lookAtText << std::endl;
			else
			{
				lookAtText = samsModule(dataFilename, "LOOK_AT");
				if (lookAtText != "END" && lookAtText != "")
					std::cout << lookAtText << std::endl;
				else
					std::cout << "It's just a " << name << "." << std::endl;
			}
		}
	}
}

void GameObject::use(GameObject* go2, Player* p)
{
	std::string useText;
	std::string obj2Name = "";

	if (go2 != NULL)
		obj2Name = go2->getName();

	if (name == "Storage Shelves" && obj2Name == "Cargo Crane")
	{
		go2->use(this, p);
	}
	else if (name == "Plasma Cutter" && obj2Name == "Collapsed Passage")
	{
		go2->use(this, p);
	}
	else if (name == "Gauss Rifle" && obj2Name == "Collapsed Passage")
	{
		go2->use(this, p);
	}
	else if (name == "Keycard" && obj2Name == "Decontamination Door")
	{
		go2->use(this, p);
	}
	else if (name == "C4 Pack" && obj2Name == "Security Gate")
	{
		go2->use(this, p);
	}
	else if (name == "Captain's Key" && obj2Name == "Blast Door")
	{
		go2->use(this, p);
	}
	else if (name == "Keyring" && obj2Name == "Armory Door")
	{
		go2->use(this, p);
	}
	else if (obj2Name == "Broken Escape Pod")
	{
		go2->use(this, p);
	}
	else if (ready == false)
	{
		if (activated == false)
		{
			if (go2 == NULL)
			{
				useText = samsModule(dataFilename, "USE");
				if (useText != "END" && useText != "")
					std::cout << useText << std::endl;
				else
					std::cout << "The " << name << " can't be used." << std::endl;

				if (name == "Communications Console")
					this->setActivated(true);		
				else if (name == "Spacesuit")
				{
					this->setActivated(true);
					this->setBeingWornByPlayer(true);
				}
			}
			else
			{
				useText = samsModule(dataFilename, "USE***" + go2->getHeaderName());
				if (useText != "END" && useText != "")
				{
					std::cout << useText << std::endl;
					//ready = true;													//this might need to be specialcase as well

																					//item by item special cases (might be better to put in special GameObject classes) 
					if (name == "Battery" && go2->getName() == "Flashlight")		//battery - flashlight interaction
					{
						p->removeItem(this);
						go2->setReady(true);
					}
					else if (name == "Flashlight" && go2->getName() == "Battery")	//battery - flashlight interaction
					{
						p->removeItem(go2);
						ready = true;
					}
					else if (go2->getName() == "Broken Escape Pod")					//escape pod - repair part interaction
					{
						p->removeItem(go2);
						go2->removeMissingPart(this);
						if (go2->isFullyRepaired() == true)
							go2->setReady(true);
					}
					//else if (name == "Gauss Rifle" && go2->getName() == "Collapsed Passageway") //medbaydoor - gaussrifle interaction
					//{
					//	go2->setActivated(true);
					//	p->getPlayerCurrentRoom()->setlockedSouth(false);
					//	std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".AFT";
					//	pickUpObject(directionFile, "true", "IS_LOCKED");
					//	pickUpObject(directionFile, "false", "IS_LOCKED");
					//	dropObject(directionFile, "false", "IS_LOCKED");
					//}
					//else if (name == "Collapsed Passageway" && go2->getName() == "Gauss Rifle") //medbaydoor - gaussrifle interaction
					//{
					//	this->setActivated(true);
					//	p->getPlayerCurrentRoom()->setlockedSouth(false);
					//	std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".AFT";
					//	pickUpObject(directionFile, "true", "IS_LOCKED");
					//	pickUpObject(directionFile, "false", "IS_LOCKED");
					//	dropObject(directionFile, "false", "IS_LOCKED");
					//}
					else if (name == "Keyring" && go2->getName() == "Armory Door") //armory door - keyring interaction
					{
						go2->setActivated(true);
						p->getPlayerCurrentRoom()->setlockedEast(false);
						std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".STARBOARD";
						pickUpObject(directionFile, "true", "IS_LOCKED");
						pickUpObject(directionFile, "false", "IS_LOCKED");
						dropObject(directionFile, "false", "IS_LOCKED");
					}
					else if (name == "Armory Door" && go2->getName() == "Keyring") //armory door - keyring interaction
					{
						this->setActivated(true);
						p->getPlayerCurrentRoom()->setlockedEast(false);
						std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".STARBOARD";
						pickUpObject(directionFile, "true", "IS_LOCKED");
						pickUpObject(directionFile, "false", "IS_LOCKED");
						dropObject(directionFile, "false", "IS_LOCKED");
					}
					else if (name == "Pen" && go2->getName() == "Paper") //pen - paper interaction
					{
						go2->setActivated(true);
					}
					else if (name == "Paper" && go2->getName() == "Pen") //pen - paper interaction
					{
						this->setActivated(true);
					}
					else if (name == "Decontamination Door" && go2->getName() == "Keycard") //decon door - keycard interaction
					{
						this->setActivated(true);
						p->getPlayerCurrentRoom()->setlockedNorth(false);
						std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".FORWARD";
						pickUpObject(directionFile, "true", "IS_LOCKED");
						pickUpObject(directionFile, "false", "IS_LOCKED");
						dropObject(directionFile, "false", "IS_LOCKED");
					}
					else if (name == "Keycard" && go2->getName() == "Decontamination Door") //decon door - keycard interaction
					{
						go2->setActivated(true);
						p->getPlayerCurrentRoom()->setlockedNorth(false);
						std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".FORWARD";
						pickUpObject(directionFile, "true", "IS_LOCKED");
						pickUpObject(directionFile, "false", "IS_LOCKED");
						dropObject(directionFile, "false", "IS_LOCKED");
					}
					else if (name == "Captain's Key" && go2->getName() == "Blast Door") //Captain's Key - Blast Door interaction
					{
						go2->setActivated(true);
						p->getPlayerCurrentRoom()->setlockedNorth(false);
						std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".FORWARD";
						pickUpObject(directionFile, "true", "IS_LOCKED");
						pickUpObject(directionFile, "false", "IS_LOCKED");
						dropObject(directionFile, "false", "IS_LOCKED");
					}
					else if (name == "Blast Door" && go2->getName() == "Captain's Key") //Captain's Key - Blast Door interaction
					{
						this->setActivated(true);
						p->getPlayerCurrentRoom()->setlockedNorth(false);
						std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".FORWARD";
						pickUpObject(directionFile, "true", "IS_LOCKED");
						pickUpObject(directionFile, "false", "IS_LOCKED");
						dropObject(directionFile, "false", "IS_LOCKED");
					}
					else if (name == "Cargo Crane" && go2->getName() == "Crane Key") //Cargo Crane - Crane Key interaction
					{
						this->setReady(true);
					}
					else if (name == "Crane Key" && go2->getName() == "Cargo Crane") //Cargo Crane - Crane Key interaction
					{
						go2->setReady(true);
					}
					else if (name == "Security Gate" && go2->getName() == "C4 Pack") //SECURITY GATE - C4 Pack interaction
					{
						p->removeItem(go2);
						this->setActivated(true);
						p->getPlayerCurrentRoom()->setlockedSouth(false);
						std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".AFT";
						pickUpObject(directionFile, "true", "IS_LOCKED");
						pickUpObject(directionFile, "false", "IS_LOCKED");
						dropObject(directionFile, "false", "IS_LOCKED");
					}
					else if (name == "C4 Pack" && go2->getName() == "Security Gate") //SECURITY GATE - C4 Pack interaction
					{
						p->removeItem(this);
						go2->setActivated(true);
						p->getPlayerCurrentRoom()->setlockedSouth(false);
						std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".AFT";
						pickUpObject(directionFile, "true", "IS_LOCKED");
						pickUpObject(directionFile, "false", "IS_LOCKED");
						dropObject(directionFile, "false", "IS_LOCKED");
					}
					else if (name == "Plasma Cutter" && go2->getName() == "Collapsed Passage") //Plasma Cutter -Collapsed Passage interaction
					{
						go2->setActivated(true);
						p->getPlayerCurrentRoom()->setlockedSouth(false);
						std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".AFT";
						pickUpObject(directionFile, "true", "IS_LOCKED");
						pickUpObject(directionFile, "false", "IS_LOCKED");
						dropObject(directionFile, "false", "IS_LOCKED");
					}
					else if (name == "Collapsed Passage" && go2->getName() == "Plasma Cutter") //Plasma Cutter -Collapsed Passage interaction
					{
						this->setActivated(true);
						p->getPlayerCurrentRoom()->setlockedSouth(false);
						std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".AFT";
						pickUpObject(directionFile, "true", "IS_LOCKED");
						pickUpObject(directionFile, "false", "IS_LOCKED");
						dropObject(directionFile, "false", "IS_LOCKED");
					}
					else if (name == "Collapsed Passage" && go2->getName() == "Gauss Rifle") //Gauss Rifle -Collapsed Passage interaction
					{
						this->setActivated(true);
						p->getPlayerCurrentRoom()->setlockedSouth(false);
						std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".AFT";
						pickUpObject(directionFile, "true", "IS_LOCKED");
						pickUpObject(directionFile, "false", "IS_LOCKED");
						dropObject(directionFile, "false", "IS_LOCKED");
					}
				}
				else
					std::cout << "The " << go2->getName() << " can't be used with the " << name << std::endl;
			}
		}
		else //activated == true
		{
			if (go2 == NULL)
			{
				useText = samsModule(dataFilename, "USE***ACTIVATED");
				if (useText != "END" && useText != "")
					std::cout << useText << std::endl;
				else
				{
					useText = samsModule(dataFilename, "USE");
					if (useText != "END" && useText != "")
						std::cout << useText << std::endl;
					else
						std::cout << "The " << name << " can't be used." << std::endl;
				}
			}
			else
			{
				useText = samsModule(dataFilename, "USE***ACTIVATED");
				if (useText != "END" && useText != "")
					std::cout << useText << std::endl;
				else
					std::cout << "The " << go2->getName() << " can't be used with the " << name << std::endl;
			}
		}

	}
	else  //ready == true
	{
		if (activated == false)
		{
			if (go2 == NULL)
			{
				useText = samsModule(dataFilename, "USE***READY");
				if (useText != "END" && useText != "")
				{
					std::cout << useText << std::endl;
					if (name == "Flashlight")
						activated = true;												//might be special case (as in flashlight and battery)
				}
				else
					std::cout << "The " << name << " can't be used." << std::endl;
			}
			else
			{
				if (name == "Cargo Crane" && go2->getName() == "Storage Shelves")
				{
					this->setActivated(true);
					go2->setReady(true);
					if (go2->getObjectInventory().getSize() > 0)
						go2->getObjectInventory().getItem(0)->setReachable(true);
					useText = samsModule(dataFilename, "USE***READY***STORAGESHELVES");
					if (useText != "END" && useText != "")
						std::cout << useText << std::endl;
				}
				else if (name == "Storage Shelves" && go2->getName() == "Cargo Crane")
				{
					go2->setActivated(true);
					this->setReady(true);
					if (this->getObjectInventory().getSize() > 0)
						this->getObjectInventory().getItem(0)->setReachable(true);
					useText = samsModule(go2->getDataFilename(), "USE***READY***STORAGESHELVES");
					if (useText != "END" && useText != "")
						std::cout << useText << std::endl;
				}
				else
					std::cout << "The " << go2->getName() << " can't be used with the " << name << std::endl;
			}
		}
		else //activated == true
		{
			if (go2 == NULL)
			{
				useText = samsModule(dataFilename, "USE***READY***ACTIVATED");
				if (useText != "END" && useText != "")
				{
					std::cout << useText << std::endl;
					if (name == "Flashlight")
						activated = false;												//might be special case (as in flashlight and battery)
				}
				else
					std::cout << "The " << name << " can't be used." << std::endl;
			}
			else
			{
				useText = samsModule(dataFilename, "USE***READY***ACTIVATED");
				if (useText != "END" && useText != "")
				{
					std::cout << useText << std::endl;
				}
				else
					std::cout << "The " << go2->getName() << " can't be used with the " << name << std::endl;
			}
		}

	}
}

void GameObject::consume(GameObject* go2, Player* p)
{
	std::string consumeText;

	if (go2 == NULL)
	{
		consumeText = samsModule(dataFilename, "CONSUME");
		if (consumeText != "END" && consumeText != "")
		{
			std::cout << consumeText << std::endl;
			p->removeItem(this);					//removes consumed item from inventory
		}
		else
			std::cout << "The " << name << " can't be consumed." << std::endl;
	}
	else
	{
		consumeText = samsModule(dataFilename, "CONSUME***" + go2->getHeaderName());
		if (consumeText != "END" && consumeText != "")
		{
			std::cout << consumeText << std::endl;
			p->removeItem(this);					//removes consumed items from inventory
			p->removeItem(go2);					//removes consumed items from inventory
		}
		else
			std::cout << "The " << go2->getName() << " can't be consumed with the " << name << std::endl;
	}
}

void GameObject::wear(GameObject* go2, Player* p)
{
	std::string wearText;

	if (go2 == NULL)
	{
		if (activated == true)
		{
			wearText = samsModule(dataFilename, "WEAR***ACTIVATED");
			if (wearText != "END" && wearText != "")
				std::cout << wearText << std::endl;
		}
		else
		{
			wearText = samsModule(dataFilename, "WEAR");
			if (wearText != "END" && wearText != "")
			{
				if (name == "Spacesuit")
				{
					this->setActivated(true);
					beingWornByPlayer = true;
					std::cout << wearText << std::endl;
				}
				else if (beingWornByPlayer == false)
				{
					std::cout << wearText << std::endl;
					beingWornByPlayer = true;
				}
				else
				{
					if (name == "Spacesuit" && p->getPlayerCurrentRoom()->getName() == "Communications Center")
					{
						std::cout << "You think better of removing your suit while in this depressurized room." << std::endl;
					}
					else
					{
						std::cout << "You are no longer wearing the " << name << "." << std::endl;
						beingWornByPlayer = false;
					}
				}
			}
			else
				std::cout << "The " << name << " can't be worn." << std::endl;
		}
	}
	else
	{
		std::cout << "Try putting on one item at a time." << std::endl;
	}
}

void GameObject::ignite(GameObject* go2, Player* p)
{
	std::string igniteText;

	if (go2 == NULL)
	{
		igniteText = samsModule(dataFilename, "IGNITE");
		if (igniteText != "END" && igniteText != "")
			std::cout << igniteText << std::endl;		//maybe also remove the item from the player inv
		else
			std::cout << "The " << name << " can't be ignited." << std::endl;
	}
	else
	{
		igniteText = samsModule(dataFilename, "IGNITE***" + go2->getHeaderName());
		if (igniteText != "END" && igniteText != "")
		{
			std::cout << igniteText << std::endl;
			if (name == "C4 Pack" && go2->getName() == "Security Gate") //SECURITY GATE - C4 Pack interaction
			{
				p->removeItem(this);
				go2->setActivated(true);
				p->getPlayerCurrentRoom()->setlockedSouth(false);
				std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".AFT";
				pickUpObject(directionFile, "true", "IS_LOCKED");
				pickUpObject(directionFile, "false", "IS_LOCKED");			
			}
			else if (name == "Security Gate" && go2->getName() == "C4 Pack") //SECURITY GATE - C4 Pack interaction
			{
				go2->removeItem(this);
				this->setActivated(true);
				p->getPlayerCurrentRoom()->setlockedSouth(false);
				std::string directionFile = p->getPlayerCurrentRoom()->getDataFilename() + ".AFT";
				pickUpObject(directionFile, "true", "IS_LOCKED");
				pickUpObject(directionFile, "false", "IS_LOCKED");			
			}
		}
		else
			std::cout << "The " << go2->getName() << " can't be ignited with the " << name << std::endl;
	}
}

void GameObject::read(GameObject* go2, Player* p)
{
	std::string readText;

	if (go2 == NULL)
	{
		readText = samsModule(dataFilename, "READ");
		if (readText != "END" && readText != "")
			std::cout << readText << std::endl;		
		else
			std::cout << "The " << name << " can't be read." << std::endl;
	}
	else
	{
		readText = samsModule(dataFilename, "READ***" + go2->getHeaderName());
		if (readText != "END" && readText != "")
		{
			std::cout << readText << std::endl;
		}
		else
			std::cout << "The " << go2->getName() << " can't be read with the " << name << std::endl;
	}
}

void GameObject::search(GameObject* go2, Player* p)
{
	std::string searchText;

	if (go2 == NULL)
	{

		if (beenSearched == false)
		{
			if (this->name == "Storage Shelves")
			{
				if (this->getReady() == true)
				{
					searchText = samsModule(dataFilename, "SEARCH***READY");
					beenSearched = true;
					this->setActivated(true);
				}
				else
					searchText = samsModule(dataFilename, "SEARCH");
			}
			else
			{
				searchText = samsModule(dataFilename, "SEARCH");
				beenSearched = true;
			}
		}
		else
			searchText = samsModule(dataFilename, "SEARCH***ACTIVATED");
		
		if (searchText != "END" && searchText != "")
		{
			std::cout << searchText << std::endl;
			if (name != "Storage Shelves" || (name == "Storage Shelves" && this->getReady() == true))
			{
				while (objectObjects.getSize() > 0)
				{
					GameObject* objectToAdd = objectObjects.getItem(0);
					if (objectToAdd->getLootable() == true)
					{
						p->addItem(objectToAdd); //add item on searched object to player inv
						objectObjects.removeItem(objectToAdd); //remove the added item from the searched object
					}
				}
			}
		}
		else
			std::cout << "The " << name << " can't be searched." << std::endl;
	}
	else
	{
		std::cout << "You can only search one thing at a time." << std::endl;
	}
}

void GameObject::combine(GameObject* go2, Player* p)
{
	std::string combineText;
	std::string obj2Name = "";

	if (go2 != NULL)
		obj2Name = go2->getName();

	if (obj2Name == "Broken Escape Pod")
	{
		go2->combine(this, p);
	}
	else if (ready == false)
	{
		if (activated == false)
		{
			if (go2 == NULL)
			{
				combineText = samsModule(dataFilename, "COMBINE");
				if (combineText != "END" && combineText != "")
					std::cout << combineText << std::endl;
				else
					std::cout << "You must specify two objects to combine." << std::endl;
			}
			else
			{
				combineText = samsModule(dataFilename, "COMBINE***" + go2->getHeaderName());
				if (combineText != "END" && combineText != "")
				{
					std::cout << combineText << std::endl;
					//ready = true;													//this might need to be specialcase as well

																					//item by item special cases (might be better to put in special GameObject classes) 
					if (name == "Battery" && go2->getName() == "Flashlight")		//battery - flashlight interaction
					{
						p->removeItem(this);
						go2->setReady(true);
					}
					else if (name == "Flashlight" && go2->getName() == "Battery")	//battery - flashlight interaction
					{
						p->removeItem(go2);
						ready = true;
					}
					else if (go2->getName() == "Broken Escape Pod")					//escape pod - repair part interaction
					{
						p->removeItem(go2);
						go2->removeMissingPart(this);
						if (go2->isFullyRepaired() == true)
							go2->setReady(true);
					}
				}
				else
					std::cout << "The " << go2->getName() << " can't be combined with the " << name << std::endl;
			}
		}
		else //activated == true
		{
			if (go2 == NULL)
			{
				combineText = samsModule(dataFilename, "COMBINE***ACTIVATED");
				if (combineText != "END" && combineText != "")
					std::cout << combineText << std::endl;
				else
				{
					combineText = samsModule(dataFilename, "COMBINE");
					if (combineText != "END" && combineText != "")
						std::cout << combineText << std::endl;
					else
						std::cout << "You must specify two objects to combine." << std::endl;
				}
			}
			else
			{
				combineText = samsModule(dataFilename, "USE***ACTIVATED");
				if (combineText != "END" && combineText != "")
					std::cout << combineText << std::endl;
				else
					std::cout << "The " << go2->getName() << " can't be combined with the " << name << std::endl;
			}
		}

	}
	else  //ready == true
	{
		if (activated == false)
		{
			if (go2 == NULL)
			{
				combineText = samsModule(dataFilename, "COMBINE***READY");
				if (combineText != "END" && combineText != "")
				{
					std::cout << combineText << std::endl;
					if (name == "Flashlight")
						activated = true;												//might be special case (as in flashlight and battery)
				}
				else
					std::cout << "You must specify two objects to combine." << std::endl;
			}
			else
				std::cout << "The " << go2->getName() << " can't be combined with the " << name << std::endl;
		}
		else //activated == true
		{
			if (go2 == NULL)
			{
				combineText = samsModule(dataFilename, "COMBINE***READY***ACTIVATED");
				if (combineText != "END" && combineText != "")
				{
					std::cout << combineText << std::endl;
					activated = false;												//might be special case (as in flashlight and battery)
				}
				else
					std::cout << "You must specify two objects to combine." << std::endl;
			}
			else
				std::cout << "The " << go2->getName() << " can't be combined with the " << name << std::endl;
		}

	}
}