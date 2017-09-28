#ifndef CAPTAINSQUARTERS_H
#define CAPTAINSQUARTERS_H

#include "Room.h"
#include "Player.h"

class CaptainsQuarters : public Room
{

public:
	CaptainsQuarters();

	virtual bool canSeeObjects(Player* p);
	virtual std::string look(Player* p);


};



#endif