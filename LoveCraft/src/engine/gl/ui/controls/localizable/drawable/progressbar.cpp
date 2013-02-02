#include "progressbar.h"

#include <cmath>
#include <string>
#include <iostream>
#include <sstream>

ProgressBar::ProgressBar() : Control(CTRLTYPE_PROGRESSBAR), m_textureBar(0), m_borderSize(0),
	m_mode(ProgressBar::BARMODE_HORIZONTAL_LTR), m_minimum(0), m_maximum(100), m_value(0)
{

}
ProgressBar::~ProgressBar()
{

}

void ProgressBar::Init(BarMode mode, Texture* bartext, bool showback, unsigned short bordersize, float max)
{
	m_mode = mode;
	m_textureBar = bartext;
	m_background = showback;
	m_borderSize = bordersize;
	m_maximum = max;

	//Lorsque la bar doit être horizontale, inversion des valeurs de largeur longueur
	//afin d'accomoder l'appel du glRotate dans le Render
	if (m_mode == ProgressBar::BARMODE_HORIZONTAL_RTL || m_mode == ProgressBar::BARMODE_VERTICAL_UTD)
	{
		m_size = Vector2i(m_size.y, m_size.x);
	}
}

void ProgressBar::Render()
{
	Render(m_texture, m_textureBar);
}

ProgressBar::BarMode ProgressBar::Mode() const
{
	return m_mode;
}
void ProgressBar::SetValue(const float value)
{
	//Validation des valeurs
	m_value = value > m_maximum ? m_maximum : (value <= m_minimum ? m_minimum : value);
}
float ProgressBar::Value() const
{
	return m_value;
}

void ProgressBar::SP(PropBool boolprop, bool value)
{
	switch (boolprop)
	{
	case PROPBOL_REPEATTEXTURE:
		Control::SP(boolprop, value);
		break;
	case PROPBOL_VISIBLE:
		Control::SP(boolprop, value);
		break;
	case PROPBOL_ENABLED:
		Control::SP(boolprop, value);
		break;
	case PROPBOL_SHOWBACKGROUND:
		m_background = value;
		break;
	default:
		assert(false);
		break;
	}
}
void ProgressBar::SP(PropFloat floatprop, float value)
{
	switch (floatprop)
	{
	case PROPFLT_BARMIN:
		assert(value <= m_maximum);
		m_minimum = value;
		break;
	case PROPFLT_BARMAX:
		assert(value >= m_minimum);
		m_maximum = value;
		break;
	default:
		assert(false);
		break;
	}
}
void ProgressBar::SP(PropUShort ushortprop, unsigned short value)
{
	switch (ushortprop)
	{
	case PROPUSHRT_BORDERSIZE:
		m_borderSize = value;
		break;
	default:
		assert(false);
		break;
	}
}
void ProgressBar::SP(PropTexture textureprop, Texture* value)
{
	switch (textureprop)
	{
	case PROPTEXT_BACKGROUND:
		Control::SP(textureprop, value);
		break;
	case PROPTEXT_BAR:
		m_textureBar = value;
		break;
	default:
		assert(false);
		break;
	}
}

bool ProgressBar::GP(PropBool boolprop) const
{
	switch (boolprop)
	{
	case PROPBOL_REPEATTEXTURE:
		return Control::GP(boolprop);
	case PROPBOL_VISIBLE:
		return Control::GP(boolprop);
	case PROPBOL_ENABLED:
		return Control::GP(boolprop);
	case PROPBOL_SHOWBACKGROUND:
		return m_background;
	default:
		assert(false);
		return false;
	}
}
float ProgressBar::GP(PropFloat floatprop) const
{
	switch (floatprop)
	{
	case PROPFLT_BARMIN:
		return m_minimum;
	case PROPFLT_BARMAX:
		return m_maximum;
	default:
		assert(false);
		return 0;
	}
}
unsigned short ProgressBar::GP(PropUShort ushortprop) const
{
	switch (ushortprop)
	{
	case PROPUSHRT_BORDERSIZE:
		return m_borderSize;
	default:
		assert(false);
		return 0;
	}
}
Texture* ProgressBar::GP(PropTexture textureprop) const
{
	switch (textureprop)
	{
	case PROPTEXT_BACKGROUND:
		return Control::GP(textureprop);
	case PROPTEXT_BAR:
		return m_textureBar;
	default:
		assert(false);
		return 0;
	}
}

// Private

void ProgressBar::Render(Texture* back, Texture* front)
{
	if (m_visible)
	{
		bool invertedBar = m_mode == BARMODE_HORIZONTAL_RTL || m_mode == BARMODE_VERTICAL_UTD;
		//Render du fond
		if (back && m_background)
			RenderSquare(AbsolutePosition() - Vector2i(m_borderSize, m_borderSize),
			m_size + Vector2i(m_borderSize * 2, m_borderSize * 2), 
			back, false);
		//Render de la bar
		if (front)
			RenderSquare(Vector2i(
			invertedBar ? AbsolutePosition().x + m_size.x - ValuePerc() : AbsolutePosition().x, 
			AbsolutePosition().y), 
			Vector2i(ValuePerc(), m_size.y), 
			front);
	}
}

float ProgressBar::ValuePerc() const
{
	return (m_value / m_maximum) * m_size.x;
}

void ProgressBar::RenderSquare(const Vector2i& position, const Vector2i& size, Texture* texture, bool repeat)
{
	if (m_blend != CBLEND_NONE)
	{
		glEnable(GL_BLEND);
		if (IsBlend(CBLEND_PNG))
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		else if (IsBlend(CBLEND_BLUR))
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	texture->Bind();
	glLoadIdentity();
	glTranslated(position.x, position.y, 0);

	//Applications des transoformations nécessaires au bon affichage du mode de bar
	ApplyRotationTransformation(m_mode, size);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(0, 0);

	glTexCoord2f(repeat ? size.x / texture->GetWidth() : 1, 0);
	glVertex2i(size.x, 0);

	glTexCoord2f(repeat ? size.x / texture->GetWidth() : 1, repeat ? size.y / texture->GetHeight() : 1);
	glVertex2i(size.x, size.y);

	glTexCoord2f(0, repeat ? size.y / texture->GetHeight() : 1);
	glVertex2i(0, size.y);

	glEnd();

	if (m_blend != CBLEND_NONE)
		glDisable(GL_BLEND);
}

void ProgressBar::ApplyRotationTransformation(BarMode type, Vector2i size) const
{
	float rotateAngle = 0;
	switch (type)
	{
	case BARMODE_HORIZONTAL_LTR:
	case BARMODE_HORIZONTAL_RTL:
		glTranslatef(0, 0, 0);
		break;
	case BARMODE_VERTICAL_DTU:
	case BARMODE_VERTICAL_UTD:
		glTranslatef(size.y, 0, 0);
		rotateAngle = 90.0f;
		break;
	}
	glRotatef(rotateAngle, 0, 0, 1.0f);
}

void ProgressBar::RenderCircle(const Vector2i& origin, float rayon, Texture* texture, bool repeat)
{

}