#ifndef SOLARIUM_H
#define SOLARIUM_H

#include "Room.h"
#include "Player.h"

class Solarium : public Room
{

public:
	Solarium();

	virtual bool canSeeObjects(Player* p);
	virtual std::string look(Player* p);
	virtual bool goDirection(std::string direction, Player* p);

};



#endif