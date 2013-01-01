#include "label.h"


Label::Label() : Control(CTRLTYPE_LABEL), m_message("Label"), m_docking(TEXTDOCK_NONE), m_italic(false),
	m_fontHeight(12), m_fontWidth(12), m_offset(Vector2f()), m_charInterval(0.66f), m_newPosition(Vector2f())
{
}

Label::Label(Control* parent, Vector2i position, Vector2i size, Texture* color, std::string message, Docking dock, 
			 bool italic, float charHeight, float charWidth, float charinterval, Vector2f offset, const std::string& name) :
Control(CTRLTYPE_LABEL, parent, position, size, color, name), m_message(message), m_docking(dock), m_italic(italic), 
	m_fontWidth(charWidth), m_fontHeight(charHeight), m_charInterval(charinterval), m_offset(offset)
{

}

Label::~Label()
{
}

void Label::SetMessage(std::string& message)
{
	m_message = message;
}

void Label::SetDock(Docking dock)
{
	m_docking = dock;
}

Label::Docking Label::GetDock() const
{
	return m_docking;
}

void Label::SetItalic(bool value)
{
	m_italic = value;
}

bool Label::GetItalic() const
{
	return m_italic;
}

void Label::SetFontWidth(float value)
{
	m_fontWidth = value;
}

float Label::GetFontWidth() const
{
	return m_fontWidth;
}

void Label::SetFontHeight(float value)
{
	m_fontHeight = value;
}

float Label::GetFontHeight() const
{
	return m_fontHeight;
}

Vector2f Label::GetOffset() const
{
	return m_offset;
}

void Label::SetOffset(Vector2f offset)
{
	m_offset = offset;
}

void Label::SetInterval(unsigned short interval)
{
	m_charInterval = (float)(interval / m_fontWidth);
}

void Label::SetInterval(float interval)
{
	m_charInterval = interval;
}

void Label::Render()
{
	if (m_visible)
	{
		if (m_texture)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			m_texture->Bind();
			glLoadIdentity();

			m_newPosition = GetNewPosition(m_message.length());

			glTranslatef(m_newPosition.x, m_newPosition.y, 0);
			for (unsigned int i = 0; i < m_message.length(); ++i)
			{
				float left = (float)((m_message[i] - 32) % 16) / 16.0f;
				float top = (float)((m_message[i] - 32) / 16) / 16.0f;
				top += m_italic ? 0.5f : 1.0f;
				glBegin(GL_QUADS);
				glTexCoord2f(left, 1.0f - top - 0.0625f);
				glVertex2i(0, 0);
				glTexCoord2f(left + 0.0625f, 1.0f - top - 0.0625f);
				glVertex2i(m_fontWidth , 0);
				glTexCoord2f(left + 0.0625f, 1.0f - top);
				glVertex2i(m_fontWidth, m_fontHeight);
				glTexCoord2f(left , 1.0f - top);
				glVertex2i(0, m_fontHeight);
				glEnd();
				glTranslatef(m_charInterval * m_fontWidth, 0, 0);
			}
			glDisable(GL_BLEND);
		}
	}
}

Label& Label::operator=(const Label& l)
{
	m_charInterval = l.m_charInterval;
	m_docking = l.m_docking;
	m_fontHeight = l.m_fontHeight;
	m_fontWidth = l.m_fontWidth;
	m_italic = l.m_italic;
	m_message = l.m_message;
	m_name = l.m_name;
	m_newPosition = l.m_newPosition;
	m_offset = l.m_offset;
	m_parent = l.m_parent;
	m_pngBlend = l.m_pngBlend;
	m_position = l.m_position;
	m_size = l.m_size;
	m_texture = l.m_texture;
	m_type = l.m_type;
	m_visible = l.m_visible;

	return *this;
}

// private

Vector2f Label::GetNewPosition(unsigned short length)
{
	Vector2f relposition;
	Vector2f absposition;

	// Vérification de l'ancrage et assignation de la position relative au parent
	switch (m_docking)
	{
	case Label::TEXTDOCK_TOPLEFT:
		relposition = Vector2f(0, m_parent->Size().y - m_fontHeight);
		break;
	case Label::TEXTDOCK_TOPCENTER:
		relposition = Vector2f((m_parent->Size().x - (length * m_charInterval * m_fontWidth)) / 2, m_parent->Size().y - m_fontHeight);
		break;
	case Label::TEXTDOCK_TOPRIGHT:
		relposition = Vector2f(m_parent->Size().x - (length * m_charInterval * m_fontWidth), m_parent->Size().y - m_fontHeight);
		break;
	case Label::TEXTDOCK_MIDDLELEFT:
		relposition = Vector2f(0, (m_parent->Size().y - m_fontHeight) / 2);
		break;
	case Label::TEXTDOCK_MIDDLECENTER:
		relposition = Vector2f((m_parent->Size().x - (length * m_charInterval * m_fontWidth)) / 2, (m_parent->Size().y - m_fontHeight) / 2);
		break;
	case Label::TEXTDOCK_MIDDLERIGHT:
		relposition = Vector2f(m_parent->Size().x - (length * m_charInterval * m_fontWidth), (m_parent->Size().y - m_fontHeight) / 2);
		break;
	case Label::TEXTDOCK_BOTTOMLEFT:
		relposition = Vector2f(0, 0);
		break;
	case Label::TEXTDOCK_BOTTOMCENTER:
		relposition = Vector2f((m_parent->Size().x - (length * m_charInterval * m_fontWidth)) / 2, 0);
		break;
	case Label::TEXTDOCK_BOTTOMRIGHT:
		relposition = Vector2f(m_parent->Size().x  / 2 - (length * m_charInterval * m_fontWidth), 0);
		break;
	default:
		relposition = Vector2f((float)m_position.x, (float)m_position.y);
		break;
	}
	//Position relative avec le offset
	relposition += m_offset;
	//Position absolue prete à être dessinée
	Vector2i& posparent = m_parent->AbsolutePosition();
	absposition = Vector2f(relposition.x + (float)posparent.x, relposition.y + (float)posparent.y);
	return absposition;
}