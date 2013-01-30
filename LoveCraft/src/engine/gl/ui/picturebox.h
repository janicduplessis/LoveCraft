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
	PictureBox();
	~PictureBox();

	void Init();
};

#endif