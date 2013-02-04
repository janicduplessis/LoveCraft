#include "localizable.h"


Localizable::Localizable(CONTROLTYPE type) : Control(type), m_blend(CBLEND_PNG), m_visible(true), m_background(0)
{
}


Localizable::~Localizable()
{
}

void Localizable::InitLocalizable(Point position, Size size, Texture* background, ORIGIN origin)
{
	m_position = position;
	m_size = size;
	m_background = background;
	SetOrigin(origin);
}

void Localizable::DrawSquare()
{
	if (m_visible)
	{
		DrawingActivateBlend();
		DrawingBindTexture();
		DrawingGetGraphicReady();
		DrawingDrawSquare();
		DrawingDesactivateBlend();
	}
}

// Propriétés

#pragma region Position

void Localizable::SetPosition(Point position)
{
	m_position = position;
}
Point Localizable::GetPosition() const
{
	return m_position;
}

bool Localizable::IsPosition(Point position) const
{
	return m_position == position;
}

Point Localizable::AbsolutePosition() const
{
	return m_position + (m_parent != 0 ? dynamic_cast<Localizable*>(m_parent)->AbsolutePosition() : Point(0, 0));
}

#pragma endregion

#pragma region Origin

void Localizable::SetOrigin(ORIGIN origin)
{
	m_origin = origin;
	m_size = Size((int)fabs((float)m_size.w), (int)fabs((float)m_size.h));
	switch (origin)
	{
	case ORIGIN_BOTTOMRIGHT:
		m_size = Size(m_size.w * -1, m_size.h);
		break;
	case ORIGIN_TOPLEFT:
		m_size = Size(m_size.w, m_size.h * -1);
		break;
	case ORIGIN_TOPRIGHT:
		m_size = Size(m_size.w * -1, m_size.h * -1);
		break;
	default:
		break;
	}
}
ORIGIN Localizable::GetOrigin() const
{
	return m_origin;
}
bool Localizable::IsOrigin(ORIGIN origin) const
{
	return m_origin == origin;
}

#pragma endregion

#pragma region Size

void Localizable::SetSize(Size size)
{
	m_size = size;
}
Size Localizable::GetSize() const
{
	return m_size;
}
bool Localizable::IsSize(Size size) const
{
	return m_size == size;
}

#pragma endregion

#pragma region Blend

void Localizable::SetBlend(BLENDTYPE btype)
{
	m_blend = btype;
}
BLENDTYPE Localizable::GetBlend() const
{
	return m_blend;
}
bool Localizable::IsBlend(BLENDTYPE btype) const
{
	return m_blend == btype;
}

#pragma endregion

#pragma region Visible

void Localizable::Hide()
{
	m_visible = false;
}
void Localizable::Show()
{
	m_visible = true;
}
bool Localizable::IsVisible() const
{
	return m_visible;
}

#pragma endregion

#pragma region Background

void Localizable::SetBackground(Texture* texture)
{
	m_background = texture;
}
void Localizable::RemoveBackground()
{
	m_background = 0;
}
Texture* Localizable::GetBackground() const
{
	return m_background;
}
bool Localizable::IsBackground(Texture* texture)
{
	return m_background == texture;
}

#pragma endregion

#pragma region Mouse Events

bool Localizable::MousePressEvents( int x, int y )
{
	return false;
}

bool Localizable::KeyPressEvents( int keycode )
{
	return false;
}

bool Localizable::KeyReleaseEvents( int keycode )
{
	return false;
}

bool Localizable::MouseReleaseEvents( int x, int y )
{
	return false;
}

#pragma endregion

#pragma region Drawing

void Localizable::DrawingActivateBlend() const
{
	if (m_blend != CBLEND_NONE)
	{
		glEnable(GL_BLEND);
		if (IsBlend(CBLEND_PNG))
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		else if (IsBlend(CBLEND_BLUR))
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
}
void Localizable::DrawingGetGraphicReady() const
{
	glLoadIdentity();
	glTranslated(m_position.x, m_position.y, 0);
}
void Localizable::DrawingBindTexture()
{
	if (m_background)
		m_background->Bind();
}
void Localizable::DrawingDrawSquare() const
{
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(0, 0);

	glTexCoord2f(1, 0);
	glVertex2i(m_size.w, 0);

	glTexCoord2f(1, 1);
	glVertex2i(m_size.w, m_size.h);

	glTexCoord2f(0, 1);
	glVertex2i(0, m_size.h);

	glEnd();
}
void Localizable::DrawingDesactivateBlend() const
{
	if (m_blend != CBLEND_NONE)
		glDisable(GL_BLEND);
}

#pragma endregion