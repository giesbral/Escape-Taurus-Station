#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <iostream>
#include <vector>
#include "Inventory.h"

class Player;

class GameObject
{
protected:
	std::string name;
	std::string longDescription;
	bool lootable;
	bool ready = false;
	bool activated = false;
	bool beenSearched = false;
	bool beingWornByPlayer = false;
	bool reachable = true;
	std::string useableWith;
	std::string dataFilename;
	std::string headerName;
	std::vector<std::string> aliases;
	Inv objectObjects;

public:
	void setName(std::string);
	void setLongDescription(std::string);
	void setLootable(bool);
	void setReady(bool);
	void setActivated(bool);
	void setSearched(bool);
	void setBeingWornByPlayer(bool);
	void setUseableWith(std::string);
	void setDataFilename(std::string);
	void setHeaderName(std::string);
	void addAlias(std::string);
	void setReachable(bool);

	std::string getName();
	std::string getLongDescription();
	bool getLootable();
	bool getReady();
	bool getActivated();
	bool getSearched();
	bool getBeingWornByPlayer();
	bool getReachable();
	std::string getDataFilename();
	std::string getHeaderName();
	std::vector<std::string> getAliases();

	Inv getObjectInventory();

	int addItem(GameObject*, bool writeToDatafile = true);		
	int removeItem(GameObject*);	

	virtual void use(GameObject*, Player*);
	virtual void lookAt(GameObject* = NULL);
	virtual void consume(GameObject* go2, Player* p);
	virtual void wear(GameObject* go2, Player* p);
	virtual void ignite(GameObject*go2, Player* p);
	virtual void read(GameObject* go2, Player* p);
	virtual void search(GameObject*, Player* p);
	virtual void combine(GameObject* go2, Player* p);

	virtual bool isFullyRepaired() { return false; };
	
	virtual void removeMissingPart(GameObject*) {};
	virtual void addMissingPart(GameObject*) {};
};

#endif