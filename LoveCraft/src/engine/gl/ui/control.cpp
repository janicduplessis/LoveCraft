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

Control& Control::operator=(const Control& c)
{
	m_name = c.m_name;
	m_parent = c.m_parent;
	m_position = c.m_position;
	m_size = c.m_size;
	m_texture = c.m_texture;
	m_type = c.m_type;
	m_visible = c.m_visible;
	m_pngBlend = c.m_pngBlend;

	return *this;
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
