#ifndef PICTUREBOX_H_
#define PICTUREBOX_H_

#include "../drawable.h"

class PictureBox : public Drawable
{
public:
	PictureBox();
	virtual ~PictureBox();

	void Init();
};

#endif