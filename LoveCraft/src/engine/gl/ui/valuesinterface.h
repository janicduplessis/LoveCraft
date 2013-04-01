#ifndef VALUES_INTERFACE_H__
#define VALUES_INTERFACE_H__

#include "define.h"
#include "engine/gl/texture.h"

struct ValuesInterface
{
	virtual void Update(const Point& _mousepos, int _width, int _height) 
	{
		if (Mousepos != _mousepos)
			Mousepos = _mousepos;
		if (Width != _width)
			Width = _width;
		if (Height != _height)
			Height = _height;
	}

	Point Mousepos;

	int Width;
	int Height;
};

#endif