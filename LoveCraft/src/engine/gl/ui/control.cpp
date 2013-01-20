#include "control.h"

Control::Control() : m_texture(0), m_parent(0), m_name(""), m_pngBlend(true), m_position(Vector2i()), m_size(Vector2i()),
	m_type(CTRLTYPE_NONE), m_visible(true), m_repeatTexture(true)
{
}

Control::Control(Type type) : m_type(type), m_visible(true), m_name("default"),
	m_parent(0), m_position(Vector2i()), m_size(Vector2i(100, 100)), m_texture(0), m_pngBlend(true), m_repeatTexture(true)
{
}

Control::Control(Type type, Control* parent, const Vector2i& position, const Vector2i& size, Texture* texture, const std::string& name) : 
	m_type(type), m_visible(true), m_name(name), m_parent(parent), m_position(position), m_texture(texture),
	m_size(size), m_pngBlend(true), m_repeatTexture(true)
{
}


Control::~Control()
{

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

std::string Control::Name() const
{
	return m_name;
}

bool Control::Visible() const
{
	return m_visible;
}
void Control::SetVisible(const bool value)
{
	m_visible = value;
}
Vector2i Control::Size() const
{
	return m_size;
}
Vector2i Control::Position() const
{
	return m_position;
}
void Control::SetSize(Vector2i value)
{
	m_size = value;
}

void Control::SetPosition(Vector2i value)
{
	m_position = value;
}

Vector2i Control::AbsolutePosition() const
{
	return m_position + (m_parent != 0 ? m_parent->AbsolutePosition() : 0);
}

void Control::SetTexture(Texture* text)
{
	m_texture = text;
}

Texture* Control::GetTexture() const
{
	return m_texture;
}

bool Control::GetPngBlend() const
{
	return m_pngBlend;
}

void Control::SetPngBlend(const bool value)
{
	m_pngBlend = value;
}
bool Control::GetRepeatTexture() const
{
	return m_repeatTexture;
}
void Control::SetRepeatTexture(const bool value)
{
	m_repeatTexture = value;
}

bool Control::MousePressEvents( int x, int y )
{
	return false;
}

void Control::SetProperty(PropBool boolprop, bool value)
{
	switch (boolprop)
	{
	case Control::PROPBOL_PNGBLEND:
		m_pngBlend = value;
		break;
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
void Control::SetProperty(PropVector2 vector2prop, Vector2i value)
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
void Control::SetProperty(PropString stringprop, std::string value)
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
void Control::SetProperty(PropTexture textureprop, Texture* value)
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

bool Control::GetProperty(PropBool boolprop) const
{
	switch (boolprop)
	{
	case Control::PROPBOL_PNGBLEND:
		return m_pngBlend;
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
Vector2i Control::GetProperty(PropVector2 vector2prop) const
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
std::string Control::GetProperty(PropString stringprop) const
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
Texture* Control::GetProperty(PropTexture textureprop) const
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

void Control::RenderSquare(const Vector2i& position, const Vector2i& size, Texture* texture)
{
	if (m_pngBlend)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
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
	if (m_pngBlend)
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