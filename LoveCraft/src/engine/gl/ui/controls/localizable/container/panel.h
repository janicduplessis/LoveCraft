#ifndef PANEL_H_
#define PANEL_H_

#include "../container.h"

class Panel : public Container
{
public:
	Panel();
	~Panel();
	virtual void Init(Texture* background);
	virtual void Render();
protected:
	virtual void DrawingBindTexture();
};

#endif