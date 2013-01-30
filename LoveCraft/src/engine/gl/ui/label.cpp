#include "label.h"


Label::Label() : Control(CTRLTYPE_LABEL), IText(), m_docking(Label::TEXTDOCK_NONE), m_offset(Vector2f()),
	m_newPosition(Vector2f())
{
}

Label::~Label()
{
}

void Label::Init(Docking dock, const Vector2f& offset)
{
	m_docking = dock;
	m_offset = offset;
}

void Label::SP(PropBool boolprop, bool value)
{
	switch (boolprop)
	{
	case Control::PROPBOL_REPEATTEXTURE:
		Control::SP(boolprop, value);
		break;
	case Control::PROPBOL_VISIBLE:
		Control::SP(boolprop, value);
		break;
	case Control::PROPBOL_ENABLED:
		Control::SP(boolprop, value);
		break;
	case Control::PROPBOL_ITALIC:
		m_italic = value;
		break;
	default:
		assert(false);
		break;
	}
}
void Label::SP(PropVector2 vector2prop, Vector2i value)
{
	switch (vector2prop)
	{
	case Control::PROPVCT2_POSITION:
		Control::SP(vector2prop, value);
		break;
	case Control::PROPVCT2_SIZE:
		Control::SP(vector2prop, value);
		break;
	case Control::PROPVCT2_TEXTOFFSET:
		m_offset = Vector2f(value.x, value.y);
		break;
	default:
		assert(false);
		break;
	}
}
void Label::SP(PropString stringprop, string value)
{
	switch (stringprop)
	{
	case Control::PROPSTR_NAME:
		Control::SP(stringprop, value);
		break;
	case Control::PROPSTR_TEXT:
		m_message = value;
		break;
	default:
		assert(false);
		break;
	}
}
void Label::SP(PropTexture textureprop, Texture* value)
{
	switch (textureprop)
	{
	case Control::PROPTEXT_BACKGROUND:
		Control::SP(textureprop, value);
		break;
	case Control::PROPTEXT_FONT_COLOR:
		m_fontColor = value;
		break;
	default:
		assert(false);
		break;
	}
}
void Label::SP(PropFloat floatprop, float value)
{
	switch (floatprop)
	{
	case Control::PROPFLT_FONTW:
		m_charWidth = value;
		break;
	case Control::PROPFLT_FONTH:
		m_charHeight = value;
		break;
	case Control::PROPFLT_FONTI:
		m_charInterval = value;
		break;
	default:
		assert(false);
		break;
	}
}
void Label::SP(PropDock dockprop, Docking value)
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

bool Label::GP(PropBool boolprop) const
{
	switch (boolprop)
	{
	case Control::PROPBOL_REPEATTEXTURE:
		return Control::GP(boolprop);
	case Control::PROPBOL_VISIBLE:
		return Control::GP(boolprop);
	case Control::PROPBOL_ENABLED:
		return Control::GP(boolprop);
	case Control::PROPBOL_ITALIC:
		return m_italic;
	default:
		assert(false);
		return false;
	}
}
Vector2i Label::GP(PropVector2 vector2prop) const
{
	switch (vector2prop)
	{
	case Control::PROPVCT2_POSITION:
		return Control::GP(vector2prop);
	case Control::PROPVCT2_SIZE:
		return Control::GP(vector2prop);
	case Control::PROPVCT2_TEXTOFFSET:
		return Vector2i((int)m_offset.x, (int)m_offset.y);
	default:
		assert(false);
		return Vector2i();
	}
}
string Label::GP(PropString stringprop) const
{
	switch (stringprop)
	{
	case Control::PROPSTR_NAME:
		return Control::GP(stringprop);
	case Control::PROPSTR_TEXT:
		return m_message;
	default:
		assert(false);
		return "";
	}
}
Texture* Label::GP(PropTexture textureprop) const
{
	switch (textureprop)
	{
	case Control::PROPTEXT_BACKGROUND:
		return Control::GP(textureprop);
	case Control::PROPTEXT_FONT_COLOR:
		return m_fontColor;
	default:
		assert(false);
		return 0;
	}
}
float Label::GP(PropFloat floatprop) const
{
	switch (floatprop)
	{
	case Control::PROPFLT_FONTW:
		return m_charWidth;
	case Control::PROPFLT_FONTH:
		return m_charHeight;
	case Control::PROPFLT_FONTI:
		return m_charInterval;
	default:
		assert(false);
		return 0.f;
	}
}
Label::Docking Label::GP(PropDock dockprop) const
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
		m_newPosition = GetNewPosition(m_message.length());

		if (m_texture)
		{
			//Dessine le fond du label s'il y en a un
			RenderSquare(Vector2i(m_newPosition.x, m_newPosition.y), 
				Vector2i(m_message.length() * m_charWidth * m_charInterval), m_texture);
		}
		if (m_fontColor)
		{
			//Dessine les lettres
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			m_fontColor->Bind();
			glLoadIdentity();

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
				glVertex2i(m_charWidth , 0);
				glTexCoord2f(left + 0.0625f, 1.0f - top);
				glVertex2i(m_charWidth, m_charHeight);
				glTexCoord2f(left , 1.0f - top);
				glVertex2i(0, m_charHeight);
				glEnd();
				glTranslatef(m_charInterval * m_charWidth, 0, 0);
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
		relposition = Vector2f(0, m_parent->GP(Control::PROPVCT2_SIZE).y - m_charHeight);
		break;
	case Label::TEXTDOCK_TOPCENTER:
		relposition = Vector2f((m_parent->GP(Control::PROPVCT2_SIZE).x - (length * m_charInterval * m_charWidth)) / 2, m_parent->GP(Control::PROPVCT2_SIZE).y - m_charHeight);
		break;
	case Label::TEXTDOCK_TOPRIGHT:
		relposition = Vector2f(m_parent->GP(Control::PROPVCT2_SIZE).x - (length * m_charInterval * m_charWidth), m_parent->GP(Control::PROPVCT2_SIZE).y - m_charHeight);
		break;
	case Label::TEXTDOCK_MIDDLELEFT:
		relposition = Vector2f(0, (m_parent->GP(Control::PROPVCT2_SIZE).y - m_charHeight) / 2);
		break;
	case Label::TEXTDOCK_MIDDLECENTER:
		relposition = Vector2f((m_parent->GP(Control::PROPVCT2_SIZE).x - (length * m_charInterval * m_charWidth)) / 2, (m_parent->GP(Control::PROPVCT2_SIZE).y - m_charHeight) / 2);
		break;
	case Label::TEXTDOCK_MIDDLERIGHT:
		relposition = Vector2f(m_parent->GP(Control::PROPVCT2_SIZE).x - (length * m_charInterval * m_charWidth), (m_parent->GP(Control::PROPVCT2_SIZE).y - m_charHeight) / 2);
		break;
	case Label::TEXTDOCK_BOTTOMLEFT:
		relposition = Vector2f(0, 0);
		break;
	case Label::TEXTDOCK_BOTTOMCENTER:
		relposition = Vector2f((m_parent->GP(Control::PROPVCT2_SIZE).x - (length * m_charInterval * m_charWidth)) / 2, 0);
		break;
	case Label::TEXTDOCK_BOTTOMRIGHT:
		relposition = Vector2f(m_parent->GP(Control::PROPVCT2_SIZE).x  / 2 - (length * m_charInterval * m_charWidth), 0);
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