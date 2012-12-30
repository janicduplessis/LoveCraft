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
}

void ProgressBar::Render(Texture& back, Texture& front)
{
	if (m_mode == BARMODE_HORIZONTAL_LTR)
	{
		//Render du fond noir
		RenderSquare(Vector2i(m_position.x - PROGRESS_BAR_OUTLINE, m_position.y - PROGRESS_BAR_OUTLINE),
			Vector2i(m_size.x + PROGRESS_BAR_OUTLINE * 2, m_size.y + PROGRESS_BAR_OUTLINE * 2), 
			back);
		//Render de la bar
		RenderSquare(m_position, 
			Vector2i(ValueWidth(), m_size.y), 
			front);
	}
}
void ProgressBar::RenderSquare(const Vector2i& position, const Vector2i& size, Texture& texture, bool repeat)
{
	texture.Bind();
	glLoadIdentity();
	glTranslated(position.x, position.y, 0);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(0, 0);

	glTexCoord2f((repeat ? size.x / texture.GetWidth() : 1), 0);
	glVertex2i(size.x, 0);

	glTexCoord2f((repeat ? size.x / texture.GetWidth() : 1), (repeat ? size.y / texture.GetHeight() : 1));
	glVertex2i(size.x, size.y);

	glTexCoord2f(0, (repeat ? size.y / texture.GetHeight() : 1));
	glVertex2i(0, size.y);

	glEnd();
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
float ProgressBar::ValueWidth() const
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