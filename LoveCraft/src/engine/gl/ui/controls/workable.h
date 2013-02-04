#ifndef WORKABLE_H__
#define WORKABLE_H__

#include "../control.h"

class Workable : public Control
{
public:
	Workable(CONTROLTYPE type);
	virtual ~Workable();
};

#endif