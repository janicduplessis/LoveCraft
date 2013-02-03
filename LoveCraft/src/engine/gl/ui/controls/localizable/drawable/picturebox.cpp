#include "picturebox.h"


PictureBox::PictureBox() : Drawable(CTRLTYPE_PICTUREBOX)
{
}


PictureBox::~PictureBox()
{
}

void PictureBox::Init()
{

}

void PictureBox::Render()
{
	DrawSquare();
}