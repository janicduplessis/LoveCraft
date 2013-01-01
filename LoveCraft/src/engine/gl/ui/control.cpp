#include "control.h"

Control::Control()
{
	m_texture = new Texture();
}

Control::Control(Type type) : m_type(type), m_visible(true), m_name("default"),
	m_parentPosition(Vector2i()), m_position(Vector2i()), m_size(Vector2i(100, 100)), m_texture(0)
{
}

Control::Control(Type type, Vector2i parent, Vector2i position, Vector2i size, Texture* texture, const std::string& name) : 
	m_type(type), m_visible(true), m_name(name), m_parentPosition(parent), m_position(position), m_texture(texture),
	m_size(size)
{
	//m_texture = texture;
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
	RenderSquare(AbsolutePosition(), m_size, texture);
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
	return Vector2i(m_parentPosition.x + m_position.x, m_parentPosition.y + m_position.y);
}

void Control::SetTexture(Texture* text)
{
	m_texture = text;
}

Texture* Control::GetTexture() const
{
	return m_texture;
}

Control& Control::operator=(const Control& c)
{
	m_name = c.m_name;
	m_parentPosition = c.m_parentPosition;
	m_position = c.m_position;
	m_size = c.m_size;
	m_texture = c.m_texture;
	m_type = c.m_type;
	m_visible = c.m_visible;

	return *this;
}

void Control::RenderSquare(const Vector2i& position, const Vector2i& size, Texture* texture)
{
	texture->Bind();
	glLoadIdentity();
	glTranslated(position.x, position.y, 0);

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

void Control::RenderSquare(const Vector2i& position, const Vector2i& size)
{
	RenderSquare(position, size, m_texture);
}