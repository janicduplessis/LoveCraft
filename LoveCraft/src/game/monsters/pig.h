#ifndef PIG_H__
#define PIG_H__

#include "../animal.h"

class Pig : public Animal
{
public:
	Pig();
	~Pig();

	virtual void Init(Player* player = 0);

private:

};


#endif // !PIG_H__
