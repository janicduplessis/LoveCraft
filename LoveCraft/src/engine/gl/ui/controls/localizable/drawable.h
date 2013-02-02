#ifndef DRAWABLE_H__
#define DRAWABLE_H__

#include "../localizable.h"

class Drawable : public Localizable
{
public:
	Drawable(CONTROLTYPE type);
	virtual ~Drawable();
	virtual void Render();
protected:
	virtual void DrawingBindTexture();
};

#endif