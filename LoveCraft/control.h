#ifndef CONTROL_H_
#define CONTROL_H_

#include "define.h"

enum Image
{
	IMAGE_BOO,
	IMAGE_RUN,
	IMAGE_CROSSHAIR,
	IMAGE_PORTRAIT,
	IMAGE_HEALTH,
	IMAGE_ENERGY,
	IMAGE_MANA,
	IMAGE_LAST
};

class Control
{
public:
	Control();
	~Control();
};

#endif