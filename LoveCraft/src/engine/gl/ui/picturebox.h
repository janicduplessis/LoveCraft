#ifndef PICTUREBOX_H_
#define PICTUREBOX_H_

#include "control.h"

class PictureBox : public Control
{
public:
	enum Imageformat
	{
		IMAGEFORMAT_STRETCH,
		IMAGEFORMAT_FIT,
		IMAGEFORMAT_TILE
	};
	PictureBox(Control* parent, Vector2i &position, Vector2i &size, Texture* image, const std::string& name);
	~PictureBox();
};

#endif