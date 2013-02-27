#include <iomanip>
#include "label.h"
#include "engine/gl/ui/controls/localizable/container.h"
#include "util/vector2.h"
#include "util/vector3.h"

Label::Label() : Textual(CTRLTYPE_LABEL), m_docking(TEXTDOCK_NONE), m_offset(Point(0, 0)), m_variableMsg("")
{
	SetBlend(CBLEND_BLUR);
}

Label::~Label()
{
}

void Label::Render()
{
	if (IsVisible() && !IsMsg(""))
	{
		string message = Replace();
		Point& abspos = AbsolutePosition();
		SetSize(Size(message.length() * m_charWidth * m_charInterval - m_charWidth * m_charInterval, m_charHeight));

		if (GetBackground())
			DrawSquare();

		if (GetColor())
		{
			DrawingActivateBlend();

			GetColor()->Bind();
			glLoadIdentity();
			glTranslatef(abspos.x, abspos.y, 0);

			for (uint16 i = 0; i < message.length(); ++i)
			{
				float left = (float)((message[i] - 32) % 16) / 16.0f;
				float top = (float)((message[i] - 32) / 16) / 16.0f;
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

#pragma region Class funtions

void Label::Init(Docking dock, Point offset)
{
	m_docking = dock;
	m_offset = offset;
	AddPosition(offset);
}

void Label::InitLocalizable(Point position, Container* parent, Localizable* anchor)
{
	Localizable::InitLocalizable(position, Size(), 0, parent);
	m_anchor = anchor;
}

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
	Localizable* parent = GetParent();
	if (!parent)
		parent = m_anchor;
	Size& size = parent->GetSize();
	int width = m_message.length() * m_charInterval * m_charWidth + m_charWidth * m_charInterval;

	// V�rification de l'ancrage et assignation de la position relative au parent
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
	//Position absolue prete � �tre dessin�e
	Point posparent = parent->AbsolutePosition();
	return relposition + posparent;
}

#pragma endregion

// Propri�t�s

#pragma region Variable message

void Label::SetVariableMsg(string variable)
{
	if (m_variableMsg == variable)
		return;
	m_variableMsg = variable;
}
void Label::SetVariableMsg(float variable)
{
	std::ostringstream ss;
	ss << std::setprecision(4) << variable;
	SetVariableMsg(ss.str());
}
void Label::SetVariableMsg(int variable)
{
	std::ostringstream ss;
	ss << variable;
	SetVariableMsg(ss.str());
}
void Label::SetVariableMsg(uint32 variable)
{
	std::ostringstream ss;
	ss << variable;
	SetVariableMsg(ss.str());
}
void Label::SetVariableMsg(bool variable)
{
	std::ostringstream ss;
	ss << (variable ? "True" : "False");
	SetVariableMsg(ss.str());
}
void Label::SetVariableMsg(Control* variable)
{
	std::ostringstream ss;
	ss << (variable ? variable->GetName() : "Aucun");
	SetVariableMsg(ss.str());
}
void Label::SetVariableMsg(Vector3<float> variable)
{
	std::ostringstream ss;
	ss << "X: " << std::setprecision(4) << std::setw(8) << std::left << variable.x << 
		" Y: " << std::setprecision(4) << std::setw(8) << std::left << variable.y << 
		" Z: " << std::setprecision(4) << std::setw(8) << std::left << variable.z;
	SetVariableMsg(ss.str());
}
void Label::SetVariableMsg(Vector2<float> variable)
{
	std::ostringstream ss;
	ss << "X: " << std::setprecision(4) << std::setw(8) << std::left << variable.x << 
		" Y: " << std::setprecision(4) << std::setw(8) << std::left << variable.y;
	SetVariableMsg(ss.str());
}
void Label::SetVariableMsg(Vector3<int> variable)
{
	std::ostringstream ss;
	ss << "X: " << std::setprecision(4) << std::setw(8) << std::left << variable.x << 
		" Y: " << std::setprecision(4) << std::setw(8) << std::left << variable.y << 
		" Z: " << std::setprecision(4) << std::setw(8) << std::left << variable.z;
	SetVariableMsg(ss.str());
}
void Label::SetVariableMsg(Vector2<int> variable)
{
	std::ostringstream ss;
	ss << "X: " << std::setprecision(4) << std::setw(8) << std::left << variable.x << 
		" Y: " << std::setprecision(4) << std::setw(8) << std::left << variable.y;
	SetVariableMsg(ss.str());
}
void Label::SetVariableMsg(Point variable)
{
	std::ostringstream ss;
	ss << "X: " << std::setprecision(4) << std::setw(8) << std::left << variable.x << 
		" Y: " << std::setprecision(4) << std::setw(8) << std::left << variable.y;
	SetVariableMsg(ss.str());
}
void Label::SetVariableMsg(Size variable)
{
	std::ostringstream ss;
	ss << "W: " << std::setprecision(4) << std::setw(8) << std::left << variable.w << 
		" H: " << std::setprecision(4) << std::setw(8) << std::left << variable.h;
	SetVariableMsg(ss.str());
}

string Label::Replace()
{
	string newmsg = m_message;
	size_t pos = 0;
	while((pos = newmsg.find("&var", pos)) != std::string::npos)
	{
		newmsg.replace(pos, 4, m_variableMsg);
		pos += m_variableMsg.length();
	}
	return newmsg;
}

#pragma endregion

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