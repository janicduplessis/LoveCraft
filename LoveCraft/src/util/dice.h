#ifndef DICE_H_
#define DICE_H_

#include <ctime>
#include <cstdlib>

class Dice
{
public:
	Dice();
	~Dice();
	int Next(int min, int max) const;
};

#endif