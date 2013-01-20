#include "label.h"


Label::Label() : Control(CTRLTYPE_LABEL), m_message("Label"), m_docking(TEXTDOCK_NONE), m_italic(false),
	m_fontHeight(12), m_fontWidth(12), m_offset(Vector2f()), m_charInterval(0.66f), m_newPosition(Vector2f())
{
}

Label::Label(Control* parent, const Vector2i& position, Texture* color, const std::string& message, Docking dock, 
			 bool italic, float charHeight, float charWidth, float charinterval, const Vector2f& offset, const std::string& name) :
Control(CTRLTYPE_LABEL, parent, position, Vector2i(), color, name), m_message(message), m_docking(dock), m_italic(italic), 
	m_fontWidth(charWidth), m_fontHeight(charHeight), m_charInterval(charinterval), m_offset(offset)
{

}

Label::~Label()
{
}

void Label::SetProperty(PropBool boolprop, bool value)
{
	switch (boolprop)
	{
	case Control::PROPBOL_PNGBLEND:
		Control::SetProperty(boolprop, value);
		break;
	case Control::PROPBOL_REPEATTEXTURE:
		Control::SetProperty(boolprop, value);
		break;
	case Control::PROPBOL_VISIBLE:
		Control::SetProperty(boolprop, value);
		break;
	case Control::PROPBOL_ENABLED:
		Control::SetProperty(boolprop, value);
		break;
	case Control::PROPBOL_ITALIC:
		m_italic = value;
		break;
	default:
		assert(false);
		break;
	}
}
void Label::SetProperty(PropVector2 vector2prop, Vector2i value)
{
	switch (vector2prop)
	{
	case Control::PROPVCT2_POSITION:
		Control::SetProperty(vector2prop, value);
		break;
	case Control::PROPVCT2_SIZE:
		Control::SetProperty(vector2prop, value);
		break;
	case Control::PROPVCT2_TEXTOFFSET:
		m_offset = Vector2f(value.x, value.y);
		break;
	default:
		assert(false);
		break;
	}
}
void Label::SetProperty(PropString stringprop, std::string value)
{
	switch (stringprop)
	{
	case Control::PROPSTR_NAME:
		Control::SetProperty(stringprop, value);
		break;
	case Control::PROPSTR_TEXT:
		m_message = value;
		break;
	default:
		assert(false);
		break;
	}
}
void Label::SetProperty(PropTexture textureprop, Texture* value)
{
	switch (textureprop)
	{
	case Control::PROPTEXT_FONT_COLOR:
		m_texture = value;
		break;
	default:
		assert(false);
		break;
	}
}
void Label::SetProperty(PropFloat floatprop, float value)
{
	switch (floatprop)
	{
	case Control::PROPFLT_FONTW:
		m_fontWidth = value;
		break;
	case Control::PROPFLT_FONTH:
		m_fontHeight = value;
		break;
	case Control::PROPFLT_FONTI:
		m_charInterval = value;
		break;
	default:
		assert(false);
		break;
	}
}
void Label::SetProperty(PropDock dockprop, Docking value)
{
	switch (dockprop)
	{
	case Label::PROPDOCK_DOCKING:
		m_docking = value;
		break;
	default:
		assert(false);
		break;
	}
}

bool Label::GetProperty(PropBool boolprop) const
{
	switch (boolprop)
	{
	case Control::PROPBOL_PNGBLEND:
		return Control::GetProperty(boolprop);
	case Control::PROPBOL_REPEATTEXTURE:
		return Control::GetProperty(boolprop);
	case Control::PROPBOL_VISIBLE:
		return Control::GetProperty(boolprop);
	case Control::PROPBOL_ENABLED:
		return Control::GetProperty(boolprop);
	case Control::PROPBOL_ITALIC:
		return m_italic;
	default:
		assert(false);
		return false;
	}
}
Vector2i Label::GetProperty(PropVector2 vector2prop) const
{
	switch (vector2prop)
	{
	case Control::PROPVCT2_POSITION:
		return Control::GetProperty(vector2prop);
	case Control::PROPVCT2_SIZE:
		return Control::GetProperty(vector2prop);
	case Control::PROPVCT2_TEXTOFFSET:
		return Vector2i((int)m_offset.x, (int)m_offset.y);
	default:
		assert(false);
		return Vector2i();
	}
}
std::string Label::GetProperty(PropString stringprop) const
{
	switch (stringprop)
	{
	case Control::PROPSTR_NAME:
		return Control::GetProperty(stringprop);
	case Control::PROPSTR_TEXT:
		return m_message;
	default:
		assert(false);
		return "";
	}
}
Texture* Label::GetProperty(PropTexture textureprop) const
{
	switch (textureprop)
	{
	case Control::PROPTEXT_FONT_COLOR:
		return m_texture;
	default:
		assert(false);
		return 0;
	}
}
float Label::GetProperty(PropFloat floatprop) const
{
	switch (floatprop)
	{
	case Control::PROPFLT_FONTW:
		return m_fontWidth;
	case Control::PROPFLT_FONTH:
		return m_fontHeight;
	case Control::PROPFLT_FONTI:
		return m_charInterval;
	default:
		assert(false);
		return 0.f;
	}
}
Label::Docking Label::GetProperty(PropDock dockprop) const
{
	switch (dockprop)
	{
	case Label::PROPDOCK_DOCKING:
		return m_docking;
	default:
		assert(false);
		return TEXTDOCK_NONE;
	}
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

// private

Vector2f Label::GetNewPosition(unsigned short length)
{
	Vector2f relposition;
	Vector2f absposition;

	// Vérification de l'ancrage et assignation de la position relative au parent
	switch (m_docking)
	{
	case Label::TEXTDOCK_TOPLEFT:
		relposition = Vector2f(0, m_parent->GetProperty(Control::PROPVCT2_SIZE).y - m_fontHeight);
		break;
	case Label::TEXTDOCK_TOPCENTER:
		relposition = Vector2f((m_parent->GetProperty(Control::PROPVCT2_SIZE).x - (length * m_charInterval * m_fontWidth)) / 2, m_parent->GetProperty(Control::PROPVCT2_SIZE).y - m_fontHeight);
		break;
	case Label::TEXTDOCK_TOPRIGHT:
		relposition = Vector2f(m_parent->GetProperty(Control::PROPVCT2_SIZE).x - (length * m_charInterval * m_fontWidth), m_parent->GetProperty(Control::PROPVCT2_SIZE).y - m_fontHeight);
		break;
	case Label::TEXTDOCK_MIDDLELEFT:
		relposition = Vector2f(0, (m_parent->GetProperty(Control::PROPVCT2_SIZE).y - m_fontHeight) / 2);
		break;
	case Label::TEXTDOCK_MIDDLECENTER:
		relposition = Vector2f((m_parent->GetProperty(Control::PROPVCT2_SIZE).x - (length * m_charInterval * m_fontWidth)) / 2, (m_parent->GetProperty(Control::PROPVCT2_SIZE).y - m_fontHeight) / 2);
		break;
	case Label::TEXTDOCK_MIDDLERIGHT:
		relposition = Vector2f(m_parent->GetProperty(Control::PROPVCT2_SIZE).x - (length * m_charInterval * m_fontWidth), (m_parent->GetProperty(Control::PROPVCT2_SIZE).y - m_fontHeight) / 2);
		break;
	case Label::TEXTDOCK_BOTTOMLEFT:
		relposition = Vector2f(0, 0);
		break;
	case Label::TEXTDOCK_BOTTOMCENTER:
		relposition = Vector2f((m_parent->GetProperty(Control::PROPVCT2_SIZE).x - (length * m_charInterval * m_fontWidth)) / 2, 0);
		break;
	case Label::TEXTDOCK_BOTTOMRIGHT:
		relposition = Vector2f(m_parent->GetProperty(Control::PROPVCT2_SIZE).x  / 2 - (length * m_charInterval * m_fontWidth), 0);
		break;
	default:
		relposition = Vector2f((float)m_position.x, (float)m_position.y);
		break;
	}
	//Position relative avec le offset
	relposition = relposition + m_offset;
	m_position = Vector2i((int)relposition.x, (int)relposition.y);
	//Position absolue prete à être dessinée
	Vector2i& posparent = m_parent->AbsolutePosition();
	absposition = Vector2f(relposition.x + (float)posparent.x, relposition.y + (float)posparent.y);
	return absposition;
}