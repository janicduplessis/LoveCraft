#include "dice.h"


Dice::Dice()
{
	std::srand((unsigned)std::time(0));
}


Dice::~Dice()
{
}

int Dice::Next(int min, int max) const
{
	int range = max - min + 1;
	return min + (int)(range * rand() / (RAND_MAX + 1));
}