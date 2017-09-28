#ifndef ARMORY_H
#define ARMORY_H

#include "Room.h"
#include "Player.h"

class Armory : public Room
{

public:
	Armory();

	virtual bool canSeeObjects(Player* p);
	virtual std::string look(Player* p);


};



#endif