#include "label.h"


Label::Label() : Textual(CTRLTYPE_LABEL), m_docking(TEXTDOCK_NONE), m_offset(Point(0, 0))
{
	SetBlend(CBLEND_BLUR);
}

Label::~Label()
{
}

void Label::Init(Docking dock, Point offset)
{
	m_docking = dock;
	m_offset = offset;
	m_position = m_position + offset;
}

void Label::Render()
{
	if (m_visible && !IsMsg(""))
	{
		Point& abspos = AbsolutePosition();
		m_size = Size(m_message.length() * m_charWidth * m_charInterval - m_charWidth * m_charInterval, m_charHeight);

		if (m_background)
			DrawSquare();

		if (m_fontColor)
		{
			DrawingActivateBlend();

			m_fontColor->Bind();
			glLoadIdentity();
			glTranslatef(abspos.x, abspos.y, 0);

			for (uint16 i = 0; i < m_message.length(); ++i)
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

			DrawingDesactivateBlend();
		}
	}
}

// Propriétés

#pragma region Docking

void Label::SetDocking(Docking dock)
{
	m_docking = dock;
}
Docking Label::GetDocking() const
{
	return m_docking;
}
bool Label::IsDocking(Docking dock) const
{
	return m_docking == dock;
}

#pragma endregion

#pragma region Offset

void Label::SetOffset(Point offset)
{
	m_offset = offset;
}
Point Label::GetOffset() const
{
	return m_offset;
}
bool Label::IsOffset(Point &offset) const
{
	return m_offset == offset;
}

#pragma endregion

void Label::UseNextDocking()
{
	int dock = m_docking;
	dock++;
	if (dock >= TEXTDOCK_LAST)
		dock = 0;
	m_docking = (Docking)dock;
}

Point Label::AbsolutePosition() const
{
	Point relposition;
	Localizable* parent = dynamic_cast<Localizable*>(m_parent);
	Size& size = parent->GetSize();
	int width = m_message.length() * m_charInterval * m_charWidth - m_charWidth * m_charInterval;

	// Vérification de l'ancrage et assignation de la position relative au parent
	switch (m_docking)
	{
	case TEXTDOCK_TOPLEFT:
		relposition = Point(0, size.h - m_charHeight);
		break;
	case TEXTDOCK_TOPCENTER:
		relposition = Point((size.w - width) / 2, size.h - m_charHeight);
		break;
	case TEXTDOCK_TOPRIGHT:
		relposition = Point(size.w - width, size.h - m_charHeight);
		break;
	case TEXTDOCK_MIDDLELEFT:
		relposition = Point(0, (size.h - m_charHeight) / 2);
		break;
	case TEXTDOCK_MIDDLECENTER:
		relposition = Point((size.w - width) / 2, (size.h - m_charHeight) / 2);
		break;
	case TEXTDOCK_MIDDLERIGHT:
		relposition = Point(size.w - width, (size.h - m_charHeight) / 2);
		break;
	case TEXTDOCK_BOTTOMLEFT:
		relposition = Point(0, 0);
		break;
	case TEXTDOCK_BOTTOMCENTER:
		relposition = Point((size.w - width) / 2, 0);
		break;
	case TEXTDOCK_BOTTOMRIGHT:
		relposition = Point(size.w - width, 0);
		break;
	default:
		relposition = m_position;
		break;
	}
	//Position relative avec le offset
	if (m_docking != TEXTDOCK_NONE)
		relposition = relposition + m_offset;
	//Position absolue prete à être dessinée
	Point posparent = parent->AbsolutePosition();
	return relposition + posparent;
}

// Preotected

void Label::DrawingBindTexture()
{
	m_background->Bind();
}