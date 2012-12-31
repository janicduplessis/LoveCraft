#include "control.h"


Control::Control(Type type) : m_type(type), m_visible(true), m_name("default"),
	m_parentPosition(Vector2i()), m_position(Vector2i()), m_size(Vector2i(100, 100))
{
}


Control::~Control()
{
}

void Control::Render()
{
	RenderSquare(m_position, m_size);
}

void Control::Render(Texture& texture)
{
	RenderSquare(m_position, m_size, texture);
}

void Control::Render(Texture& textureBack, Texture& textureFront)
{
	RenderSquare(Vector2i(m_position.x + 10, m_position.y + 10), m_size, textureBack);
	RenderSquare(m_position, m_size, textureFront);
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

void Control::RenderSquare(const Vector2i& position, const Vector2i& size, Texture& texture)
{
	texture.Bind();
	glLoadIdentity();
	glTranslated(position.x, position.y, 0);

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

void Control::RenderSquare(const Vector2i& position, const Vector2i& size)
{
	glLoadIdentity();
	glTranslated(position.x, position.y, 0);

	glBegin(GL_QUADS);

	glVertex2f(0, 0);
	glVertex2i(size.x, 0);
	glVertex2i(size.x, size.y);
	glVertex2i(0, size.y);

	glEnd();
}