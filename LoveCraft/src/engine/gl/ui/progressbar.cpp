#include "progressbar.h"

#include <cmath>
#include <string>
#include <iostream>
#include <sstream>

ProgressBar::ProgressBar() : Control(CTRLTYPE_PROGRESSBAR)
{
	m_textureBack = new Texture();
}

ProgressBar::~ProgressBar()
{
	
}

ProgressBar::ProgressBar(Vector2i parent, Vector2i &position, Vector2i &size, Texture* textureFront, Texture* textureBack, BarMode mode, const std::string& name) : 
	Control(CTRLTYPE_PROGRESSBAR, parent, position, size, textureFront, name), 
	m_minimum(0), m_maximum(100), m_value(80), m_mode(mode), m_textureBack(textureBack)
{
	//Lorsque la bar doit être horizontale, inversion des valeurs de largeur longueur
	//afin d'accomoder l'appel du glRotate dans le Render
	if (m_mode == ProgressBar::BARMODE_VERTICAL_DTU || m_mode == ProgressBar::BARMODE_VERTICAL_UTD)
	{
		m_size = Vector2i(size.y, size.x);
	}
}

void ProgressBar::Render(Texture* back, Texture* front)
{
	if (Control::m_visible)
	{
		bool invertedBar = m_mode == BARMODE_HORIZONTAL_RTL || m_mode == BARMODE_VERTICAL_UTD;
		//Render du fond noir
		RenderSquare(Vector2i(
			m_parentPosition.x + m_position.x - PROGRESS_BAR_OUTLINE, 
			m_parentPosition.y + m_position.y - PROGRESS_BAR_OUTLINE),
			Vector2i(m_size.x + PROGRESS_BAR_OUTLINE * 2, m_size.y + PROGRESS_BAR_OUTLINE * 2), 
			back);
		//Render de la bar
		RenderSquare(Vector2i(
			invertedBar ? m_parentPosition.x + m_position.x + m_size.x - ValuePerc() : m_parentPosition.x + m_position.x, 
			m_parentPosition.y + m_position.y), 
			Vector2i(ValuePerc(), m_size.y), 
			front);
	}
}
void ProgressBar::Render()
{
	if (m_visible)
	{
		bool invertedBar = m_mode == BARMODE_HORIZONTAL_RTL || m_mode == BARMODE_VERTICAL_UTD;
		//Render du fond noir
		RenderSquare(Vector2i(
			AbsolutePosition().x - PROGRESS_BAR_OUTLINE, 
			AbsolutePosition().y - PROGRESS_BAR_OUTLINE),
			Vector2i(m_size.x + PROGRESS_BAR_OUTLINE * 2, m_size.y + PROGRESS_BAR_OUTLINE * 2), 
			m_textureBack);
		//Render de la bar
		RenderSquare(Vector2i(
			invertedBar ? AbsolutePosition().x + m_size.x - ValuePerc() : AbsolutePosition().x, 
			AbsolutePosition().y), 
			Vector2i(ValuePerc(), m_size.y), 
			m_texture);
	}
}
void ProgressBar::RenderSquare(const Vector2i& position, const Vector2i& size, Texture* texture)
{
	texture->Bind();
	glLoadIdentity();
	glTranslated(position.x, position.y, 0);

	//Applications des transoformations nécessaires au bon affichage du mode de bar
	ApplyRotationTransformation(m_mode, size);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(0, 0);

	glTexCoord2f(size.x / texture->GetWidth(), 0);
	glVertex2i(size.x, 0);

	glTexCoord2f(size.x / texture->GetWidth(), size.y / texture->GetHeight());
	glVertex2i(size.x, size.y);

	glTexCoord2f(0, size.y / texture->GetHeight());
	glVertex2i(0, size.y);

	glEnd();
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

ProgressBar& ProgressBar::operator=(const ProgressBar& pgb)
{
	m_minimum = pgb.m_minimum;
	m_maximum = pgb.m_maximum;
	m_mode = pgb.m_mode;
	m_textureBack = pgb.m_textureBack;
	m_value = pgb.m_value;
	m_name = pgb.m_name;
	m_parentPosition = pgb.m_parentPosition;
	m_position = pgb.m_position;
	m_size = pgb.m_size;
	m_texture = pgb.m_texture;
	m_type = pgb.m_type;
	m_visible = pgb.m_visible;

	return *this;
}