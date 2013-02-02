#include "drawable.h"


Drawable::Drawable(CONTROLTYPE type) : Localizable(type)
{
}


Drawable::~Drawable()
{
}

void Drawable::Render()
{
	DrawSquare();
}

void Drawable::DrawingBindTexture()
{
	m_background->Bind();
}