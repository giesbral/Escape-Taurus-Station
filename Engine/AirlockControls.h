#ifndef AIRLOCKCONTROLS_HPP
#define AIRLOCKCONTROLS_HPP

#include "GameObject.h"
#include "Inventory.h"

class Player;

class AirlockControls : public GameObject 
{
protected:

public:
	virtual void use(GameObject* go2, Player* p);

};
	

#endif
