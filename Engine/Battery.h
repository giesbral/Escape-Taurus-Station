#ifndef BATTERY_H
#define BATTERY_H

#include "GameObject.h"



class Battery : public GameObject
{


public:
	Battery();

	virtual void use(GameObject* = NULL, Player* = NULL);
	virtual void consume(GameObject* = NULL);


};

#endif