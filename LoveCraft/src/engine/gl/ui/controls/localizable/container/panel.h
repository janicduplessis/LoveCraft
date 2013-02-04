#ifndef PANEL_H_
#define PANEL_H_

#include "../container.h"

class Panel : public Container
{
public:
	Panel();
	~Panel();
	virtual void Render();
};

#endif