#include "localizable.h"
#include "engine/gl/ui/controls/localizable/container.h"

Localizable::Localizable(CONTROLTYPE type, CONTROLGENERICTYPE gentype) : Control(type), m_blend(CBLEND_PNG), m_visible(true), 
	m_parent(0), m_background(0), m_effects(0), m_effectNbr(0), m_gentype(gentype)
{
}

Localizable::~Localizable()
{
}

void Localizable::InitLocalizable(Point position, Size size, Texture* background, Container* parent, ORIGIN origin)
{
	m_parent = parent;
	m_position = position;
	m_size = size;
	m_background = background;
	SetOrigin(origin);
	if (parent)
		parent->AddControl(this);
}

void Localizable::DrawSquare()
{
	if (IsVisible())
	{
		DrawingActivateBlend();
		DrawingBindTexture();
		DrawingGetGraphicReady();
		DrawingDrawSquare();
		DrawingDesactivateBlend();
	}
}

// Propriétés

#pragma region Parent

void Localizable::SetParent(Container* parent)
{
	m_parent = parent;
}
Container* Localizable::GetParent() const
{
	return m_parent;
}
bool Localizable::IsParent(Container* parent)
{
	return m_parent == parent;
}

#pragma endregion

#pragma region Generic type

CONTROLGENERICTYPE Localizable::GetGenType() const
{
	return m_gentype;
}
bool Localizable::IsGenType(CONTROLGENERICTYPE gentype) const
{
	return m_gentype == gentype;
}

#pragma endregion

#pragma region Position

void Localizable::SetPosition(Point position)
{
	m_position = position;
}
void Localizable::AddPosition(Point value)
{
	m_position = m_position + value;
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
	return m_position + (m_parent ? m_parent->AbsolutePosition() : Point(0, 0));
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
void Localizable::AddSize(Size value)
{
	m_size = m_size + value;
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

#pragma region Effects

void Localizable::AddEffect(Effect* effect)
{
	if (!effect)
		return;

	Effect** effects = m_effects;
	m_effects = new Effect*[++m_effectNbr];
	for (uint8 i = 0; i < m_effectNbr - 1; i++)
		m_effects[i] = effects[i];
	m_effects[m_effectNbr - 1] = effect;
	delete [] effects;
}

#pragma endregion

#pragma region Mouse Events

bool Localizable::MouseMoveEvents(int x, int y)
{
	return IsWithinRange(x, y);
}

bool Localizable::MousePressEvents(int x, int y)
{
	return false;
}

bool Localizable::KeyPressEvents(int keycode)
{
	return false;
}

bool Localizable::KeyReleaseEvents(int keycode)
{
	return false;
}

bool Localizable::MouseReleaseEvents(int x, int y)
{
	return false;
}

#pragma endregion

#pragma region Cursor position

bool Localizable::IsWithinRange(int x, int y) const
{
	Point& fstpos = AbsolutePosition();
	Point& sndpos = Point(fstpos.x + GetSize().w, fstpos.y + GetSize().h);
	if (x >= fstpos.x && x <= sndpos.x && y >= fstpos.y && y <= sndpos.y)
		return true;
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
	Point& abspos = AbsolutePosition();
	glLoadIdentity();
	glTranslated(abspos.x, abspos.y, 0);
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