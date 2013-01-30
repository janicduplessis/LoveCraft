#include "progressbar.h"

#include <cmath>
#include <string>
#include <iostream>
#include <sstream>

ProgressBar::ProgressBar() : Control(CTRLTYPE_PROGRESSBAR)
{
	m_textureBack = 0;
}
ProgressBar::~ProgressBar()
{

}

ProgressBar::ProgressBar(Control* parent, Vector2i &position, Vector2i &size, Texture* textureFront, 
						 Texture* textureBack, BarMode mode, bool back, unsigned short border, const std::string& name) : 
Control(CTRLTYPE_PROGRESSBAR, parent, position, size, textureFront, name), 
	m_minimum(0), m_maximum(100), m_value(80), m_mode(mode), m_textureBack(textureBack), m_background(back), m_borderSize(border)
{
	//Lorsque la bar doit être horizontale, inversion des valeurs de largeur longueur
	//afin d'accomoder l'appel du glRotate dans le Render
	if (m_mode == ProgressBar::BARMODE_HORIZONTAL_RTL || m_mode == ProgressBar::BARMODE_VERTICAL_UTD)
	{
		m_size = Vector2i(size.y, size.x);
	}
}

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
void ProgressBar::Render()
{
	Render(m_textureBack, m_texture);
}
void ProgressBar::RenderSquare(const Vector2i& position, const Vector2i& size, Texture* texture, bool repeat)
{
	if (m_blend != Control::CBLEND_NONE)
	{
		glEnable(GL_BLEND);
		if (IsBlend(Control::CBLEND_PNG))
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		else if (IsBlend(Control::CBLEND_BLUR))
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

	if (m_blend != Control::CBLEND_NONE)
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

float ProgressBar::Maximum() const
{
	return m_maximum;
}
float ProgressBar::Minimum() const
{
	return m_minimum;
}
float ProgressBar::ValuePerc() const
{
	return (m_value / m_maximum) * m_size.x;
}

bool ProgressBar::GetBeckground() const
{
	return m_background;
}
void ProgressBar::SetBackgroundTo(const bool value)
{
	m_background = value;
}

unsigned short ProgressBar::GetBordersize() const
{
	return m_borderSize;
}
void ProgressBar::SetBorderSize(unsigned short size)
{
	m_borderSize = size;
}

void ProgressBar::SP(PropBool boolprop, bool value)
{
	Control::SP(boolprop, value);
}
void ProgressBar::SP(PropVector2 vector2prop, Vector2i value)
{
	Control::SP(vector2prop, value);
}
void ProgressBar::SP(PropString stringprop, std::string value)
{
	Control::SP(stringprop, value);
}
void ProgressBar::SP(PropTexture textureprop, Texture* value)
{
	switch (textureprop)
	{
	case Control::PROPTEXT_BACKGROUND:
		m_textureBack = value;
		break;
	case Control::PROPTEXT_BAR:
		m_texture = value;
		break;
	default:
		assert(false);
		break;
	}
}



bool ProgressBar::GP(PropBool boolprop) const
{
	return Control::GP(boolprop);
}
Vector2i ProgressBar::GP(PropVector2 vector2prop) const
{
	return Control::GP(vector2prop);
}
std::string ProgressBar::GP(PropString stringprop) const
{
	return Control::GP(stringprop);
}
Texture* ProgressBar::GP(PropTexture textureprop) const
{
	switch (textureprop)
	{
	case Control::PROPTEXT_BACKGROUND:
		return m_textureBack;
	case Control::PROPTEXT_BAR:
		return m_texture;
	default:
		assert(false);
		return 0;
	}
}