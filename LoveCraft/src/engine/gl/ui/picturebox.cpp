#include "picturebox.h"


PictureBox::PictureBox(Control* parent, Vector2i &position, Vector2i &size, Texture* image, const std::string& name) : 
	Control(CTRLTYPE_PICTUREBOX, parent, position, size, image, name)
{
}


PictureBox::~PictureBox()
{
}

void PictureBox::SetTexture( Texture* tex )
{
	m_texture = tex;
}
