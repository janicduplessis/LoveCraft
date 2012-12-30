#include "progressbar.h"

#include <cmath>
#include <string>
#include <iostream>
#include <string>
#include <sstream>

ProgressBar::ProgressBar()
{
}

ProgressBar::~ProgressBar()
{
}

ProgressBar::ProgressBar(Vector2i &size, Vector2i &position, BarMode mode) : 
	m_minimum(0), m_maximum(100), m_value(80), m_size(size), m_position(position), m_mode(mode)
{
	//std::ostringstream ss;
	//ss << TEXTURE_PATH << textbackfilename;
	//if (!m_textBack.Load(ss.str()))
	//	std::cout << "Erreur lors du chargement des textures de progressbar" << ss.str() << std::endl;
	//ss.str("");
	//ss << TEXTURE_PATH << textfrontfilename;
	//if (!m_textBack.Load(ss.str()))
	//	std::cout << "Erreur lors du chargement des textures de progressbar: " << ss.str() << std::endl;

	//Lorsque la bar doit être horizontale, inversion des valeurs de largeur longueur
	//afin d'accomoder l'appel du glRotate dans le Render
	if (m_mode == ProgressBar::BARMODE_VERTICAL_DTU || m_mode == ProgressBar::BARMODE_VERTICAL_DTU)
	{
		m_size = Vector2i(size.y, size.x);
	}
}

void ProgressBar::Render(Texture& back, Texture& front)
{
	//Render du fond noir
	RenderSquare(Vector2i(m_position.x - PROGRESS_BAR_OUTLINE, m_position.y - PROGRESS_BAR_OUTLINE),
		Vector2i(m_size.x + PROGRESS_BAR_OUTLINE * 2, m_size.y + PROGRESS_BAR_OUTLINE * 2), 
		back, (m_mode == BARMODE_VERTICAL_DTU || m_mode == BARMODE_VERTICAL_UTD));
	//Render de la bar
	RenderSquare(m_position, 
		Vector2i(ValuePerc(), m_size.y), 
		front, (m_mode == BARMODE_VERTICAL_DTU || m_mode == BARMODE_VERTICAL_UTD));
	////Render du fond noir
	//RenderSquare(Vector2i(m_position.x - PROGRESS_BAR_OUTLINE, m_position.y - PROGRESS_BAR_OUTLINE),
	//	Vector2i(m_size.x + PROGRESS_BAR_OUTLINE * 2, m_size.y + PROGRESS_BAR_OUTLINE * 2), 
	//	back, true);
	////Render de la bar
	//RenderSquare(m_position, 
	//	Vector2i(m_size.x, ValuePerc()), 
	//	front, true);
}
void ProgressBar::RenderSquare(const Vector2i& position, const Vector2i& size, Texture& texture, bool rotate)
{
	texture.Bind();
	glLoadIdentity();
	glTranslated(position.x, position.y, 0);

	//Applications des transoformations nécessaires au bon affichage du mode de bar
	ApplyRotationTransformation(m_mode, size);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(0, 0);

	glTexCoord2f(size.x / texture.GetWidth(), 0);
	glVertex2i(size.x, 0);

	glTexCoord2f(size.x / texture.GetWidth(), size.y / texture.GetHeight());
	glVertex2i(size.x, size.y);

	glTexCoord2f(0, size.y / texture.GetHeight());
	glVertex2i(0, size.y);

	glEnd();
}
void ProgressBar::ApplyRotationTransformation(BarMode type, Vector2i size) const
{
	switch (type)
	{
	case BARMODE_HORIZONTAL_LTR:
		glTranslatef(0, 0, 0);
		break;
	case BARMODE_VERTICAL_DTU:
		glTranslatef(size.y, 0, 0);
		break;
	case BARMODE_HORIZONTAL_RTL:
		glTranslatef(size.x, size.y, 0);
		break;
	case BARMODE_VERTICAL_UTD:
		glTranslatef(0, size.y, 0);
		break;
	}
	glRotatef((float)type, 0, 0, 1.0f);
}

void ProgressBar::RenderCircle(const Vector2i& origin, float rayon, Texture& texture, bool repeat)
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
Vector2i ProgressBar::Size() const
{
	return m_size;
}
Vector2i ProgressBar::Position() const
{
	return m_position;
}