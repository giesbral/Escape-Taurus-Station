#ifndef ESCAPEPOD_HPP
#define ESCAPEPOD_HPP

#include "GameObject.h"
#include "Inventory.h"

class Player;

class EscapePod : public GameObject 
{
protected:
	Inv missingParts;

public:

	virtual bool isFullyRepaired();
	
	Inv getMissingParts() { return missingParts; };
	virtual void removeMissingPart(GameObject*);
	virtual void addMissingPart(GameObject*);

	void printMissingParts();

	virtual void combine(GameObject* go2, Player* p);
	virtual void use(GameObject* go2, Player* p);
};
	

#endif
