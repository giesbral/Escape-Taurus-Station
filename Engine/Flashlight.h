#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H

#include "GameObject.h"
#include "Inventory.h"


class Flashlight : public GameObject
{
protected:
	Inv inv;

public:
	Flashlight();

	virtual void use(GameObject* = NULL, Player* = NULL);
	virtual void consume(GameObject* = NULL);


};

#endif