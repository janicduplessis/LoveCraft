#include "control.h"

Control::Control(Type type) : Structure(), m_type(type), m_visible(true), m_name("default"),
	m_parent(0), m_position(Vector2i()), m_size(Vector2i(100, 100)), m_texture(0), 
	m_blend(Control::CBLEND_PNG), m_repeatTexture(false), m_enabled(true)
{
}

Control::~Control()
{

}

void Control::CtrlInit(Control* parent, Vector2i &position, Vector2i &size, Texture* texture, const string &name)
{
	if (!m_initialized)
	{
		m_parent = parent;
		m_position = position;
		m_size = size;
		m_texture = texture;
		m_name = name;
		Structure::Init();
	}
}

void Control::Render()
{
	Render(m_texture);
}

void Control::Render(Texture* texture)
{
	if (m_visible)
	{
		if (texture)
			RenderSquare(AbsolutePosition(), m_size, texture);
	}
}

Vector2i Control::AbsolutePosition() const
{
	return m_position + (m_parent != 0 ? m_parent->AbsolutePosition() : 0);
}

bool Control::MousePressEvents( int x, int y )
{
	return false;
}

// =============================================================================

void Control::SP(PropBool boolprop, bool value)
{
	switch (boolprop)
	{
	case Control::PROPBOL_REPEATTEXTURE:
		m_repeatTexture = value;
		break;
	case Control::PROPBOL_VISIBLE:
		m_visible = value;
		break;
	case Control::PROPBOL_ENABLED:
		m_enabled = value;
		break;
	default:
		assert(false);
		break;
	}
}
void Control::SP(PropVector2 vector2prop, Vector2i value)
{
	switch (vector2prop)
	{
	case Control::PROPVCT2_POSITION:
		m_position = value;
		break;
	case Control::PROPVCT2_SIZE:
		m_size = value;
		break;
	default:
		assert(false);
		break;
	}
}
void Control::SP(PropString stringprop, string value)
{
	switch (stringprop)
	{
	case Control::PROPSTR_NAME:
		m_name = value;
		break;
	default:
		assert(false);
		break;
	}
}
void Control::SP(PropTexture textureprop, Texture* value)
{
	switch (textureprop)
	{
	case Control::PROPTEXT_BACKGROUND:
		m_texture = value;
		break;
	default:
		assert(false);
		break;
	}
}
void Control::SetBlend(BlendType btype)
{
	m_blend = btype;
}

// =============================================================================

bool Control::GP(PropBool boolprop) const
{
	switch (boolprop)
	{
	case Control::PROPBOL_REPEATTEXTURE:
		return m_repeatTexture;
	case Control::PROPBOL_VISIBLE:
		return m_visible;
	case Control::PROPBOL_ENABLED:
		return m_enabled;
	default:
		assert(false);
		return false;
	}
}
Vector2i Control::GP(PropVector2 vector2prop) const
{
	switch (vector2prop)
	{
	case Control::PROPVCT2_POSITION:
		return m_position;
	case Control::PROPVCT2_SIZE:
		return m_size;
	default:
		assert(false);
		return Vector2i();
	}
}
string Control::GP(PropString stringprop) const
{
	switch (stringprop)
	{
	case Control::PROPSTR_NAME:
		return m_name;
	default:
		assert(false);
		return "";
	}
}
Texture* Control::GP(PropTexture textureprop) const
{
	switch (textureprop)
	{
	case Control::PROPTEXT_BACKGROUND:
		return m_texture;
	default:
		assert(false);
		return 0;
	}
}
Control::BlendType Control::GetBlend() const
{
	return m_blend;
}
bool Control::IsBlend(BlendType btype) const
{
	return m_blend == btype;
}

// =============================================================================

void Control::RenderSquare(const Vector2i& position, const Vector2i& size, Texture* texture)
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

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(0, 0);

	glTexCoord2f(m_repeatTexture ? (size.x / texture->GetWidth()) : 1, 0);
	glVertex2i(size.x, 0);

	glTexCoord2f(m_repeatTexture ? (size.x / texture->GetWidth()) : 1, m_repeatTexture ? (size.y / texture->GetHeight()) : 1);
	glVertex2i(size.x, size.y);

	glTexCoord2f(0, m_repeatTexture ? (size.y / texture->GetHeight()) : 1);
	glVertex2i(0, size.y);

	glEnd();
	if (m_blend != Control::CBLEND_NONE)
		glDisable(GL_BLEND);
}

void Control::RenderSquare(const Vector2i& position, const Vector2i& size)
{
	RenderSquare(position, size, m_texture);
}

bool Control::KeyPressEvents( int keycode )
{
	return false;
}

bool Control::KeyReleaseEvents( int keycode )
{
	return false;
}

bool Control::MouseReleaseEvents( int x, int y )
{
	return false;
}