#ifndef PIG_H__
#define PIG_H__

#include "npc.h"

class Pig : public Npc
{
public:
	Pig();
	~Pig();

	virtual void Init(Player* player = 0);

private:

};


#endif // !PIG_H__
