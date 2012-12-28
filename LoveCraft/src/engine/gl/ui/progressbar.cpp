#include "progressbar.h"

#include <cmath>
#include <string>

ProgressBar::ProgressBar() : m_minimum(0), m_maximum(100), m_value(80), 
	m_size(Vector2i(20, 300)), m_position(Vector2i(0, 100))
{
}


ProgressBar::~ProgressBar()
{
}

ProgressBar::ProgressBar(Vector2i &size, Vector2i &position) : m_minimum(0), m_maximum(100), 
	m_value(80), m_size(size), m_position(position)
{
}

void ProgressBar::Render(const Vector2i &position)
{

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