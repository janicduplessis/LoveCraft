#include "progressbar.h"
#include "../../../uiimage.h"
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>

ProgressBar::ProgressBar() : Drawable(CTRLTYPE_PROGRESSBAR), m_foreground(CUSTIMAGE_NONE), m_borderSize(0),
	m_mode(ProgressBar::BARMODE_HORIZONTAL_LTR), m_minimum(0), m_maximum(100), m_value(0)
{
	m_backImage = new PictureBox();
}

ProgressBar::~ProgressBar()
{
	delete m_backImage;
}

void ProgressBar::Render()
{
	if (IsVisible())
	{
		m_backImage->Render();
		DrawSquare();
	}
}

void ProgressBar::DrawSquare()
{
	DrawingActivateBlend();
	DrawingGetGraphicReady();
	DrawingBindTexture();
	switch (m_mode)
	{
	case ProgressBar::BARMODE_VERTICAL_DTU:
		DrawingDrawSquareDTU();
		break;
	case ProgressBar::BARMODE_CIRCLE_ACLKW:
	case ProgressBar::BARMODE_CIRCLE_CLKW:
	case ProgressBar::BARMODE_VERTICAL_UTD:
	case ProgressBar::BARMODE_HORIZONTAL_RTL:
	case ProgressBar::BARMODE_HORIZONTAL_LTR:
		DrawingDrawSquareLTR();
		break;
	default:
		DrawingDrawSquareLTR();
		break;
	}
	DrawingDesactivateBlend();
}

void ProgressBar::DrawingBindTexture()
{
	GetForegroundTexture()->Bind();
}

#pragma region Class functions

void ProgressBar::Init(BarMode mode, IMAGE foreground, uint8 bordersize)
{
	m_mode = mode;
	m_foreground = foreground;
	m_borderSize = bordersize;

	m_backImage->InitControl("backimage");
	m_backImage->InitLocalizable(Point(-bordersize, -bordersize) + AbsolutePosition(),
		Size(GetSize().w + bordersize * 2, GetSize().h + bordersize * 2), GetBackground(), 0);
}

void ProgressBar::DrawingDrawSquareLTR()
{
	float filled = FilledWidth();
	Texture* text = GetForegroundTexture();
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(0, 0);

	glTexCoord2f(filled / text->GetWidth(), 0);
	glVertex2f(filled, 0);

	glTexCoord2f(filled / text->GetWidth(), 1);
	glVertex2f(filled, GetSize().h);

	glTexCoord2f(0, 1);
	glVertex2f(0, GetSize().h);

	glEnd();
}

void ProgressBar::DrawingDrawSquareDTU()
{
	float filled = FilledHeight();
	Texture* text = GetForegroundTexture();
	glBegin(GL_QUADS);

	glTexCoord2f(filled / text->GetWidth(), 0);
	glVertex2f(0, 0);

	glTexCoord2f(filled / text->GetWidth(), 1);
	glVertex2f(GetSize().w, 0);

	glTexCoord2f(0, 1);
	glVertex2f(GetSize().w, filled);

	glTexCoord2f(0, 0);
	glVertex2f(0, filled);

	glEnd();
}

void ProgressBar::RenderCircle(const Vector2i& origin, float rayon, Texture* texture, bool repeat)
{

}

#pragma endregion

// Propriétés

#pragma region Mode

ProgressBar::BarMode ProgressBar::GetMode() const
{
	return m_mode;
}
bool ProgressBar::IsMode(BarMode mode) const
{
	return m_mode;
}

#pragma endregion

#pragma region Value

void ProgressBar::SetValue(float value)
{
	if ((int)m_value == (int)value)
		return;
	m_value = value > m_maximum ? m_maximum : (value < m_minimum ? m_minimum : value);
}
void ProgressBar::AddValue(float value)
{
	SetValue(GetValue() + value);
}
float ProgressBar::GetValue() const
{
	return m_value;
}
bool ProgressBar::IsValue(float value) const
{
	return fabs(m_value) - fabs(value) < 0.01f;
}
bool ProgressBar::IsValue(float first, float second) const
{
	return m_value >= first && m_value <= second;
}

#pragma endregion

#pragma region Foreground

void ProgressBar::SetForeground(IMAGE foreground)
{
	if (m_foreground == foreground)
		return;
	m_foreground = foreground;
}
IMAGE ProgressBar::GetForeground() const
{
	return m_foreground;
}
bool ProgressBar::IsForeground(IMAGE foreground)
{
	return m_foreground == foreground;
}
Texture* ProgressBar::GetForegroundTexture() const
{
	return m_foreground != CUSTIMAGE_NONE ? UIImage::Get(m_foreground) : 0;
}

#pragma endregion

#pragma region Filled

float ProgressBar::FilledWidth() const
{
	return (m_value / m_maximum) * m_size.w;
}
float ProgressBar::FilledHeight() const
{
	return (m_value / m_maximum) * m_size.h;
}
float ProgressBar::FilledRadian() const
{
	return (m_value / m_maximum) * 360;
}

#pragma endregion