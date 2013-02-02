#include "listbox.h"
#include <sstream>
#include "engine/info.h"

ListBox::ListBox() : Control(CTRLTYPE_LISTBOX), IText(), m_lines(0), m_lineNbr(0), m_gapBetLines(0), 
	m_curLineIndex(0), m_scrollable(false), m_upArrow(0), m_downArrow(0)
{

}

ListBox::~ListBox()
{
	for (unsigned short i = 0; i < m_lineNbr; i++)
	{
		delete m_lines[i];
	}
	delete [] m_lines;
	delete m_upArrow;
	delete m_downArrow;
}

void ListBox::Init(unsigned short lineNbr, short gap, Vector2i offset, bool scrollable)
{
	Init(lineNbr, gap, offset, scrollable, 0, 0);
}

void ListBox::Init(unsigned short lineNbr, short gap, Vector2i offset, bool scrollable, Texture* uptext, Texture* downtext)
{
	m_lineNbr = lineNbr;
	m_gapBetLines = gap;
	m_scrollable = scrollable;
	m_updownButtons = uptext != 0 && downtext != 0;

	m_size = Vector2i(m_size.x + offset.x * 2, (m_charHeight + gap) * lineNbr + offset.y * 2);

	m_lines = new Label*[lineNbr];
	std::ostringstream ss;
	for (unsigned short i = 0; i < m_lineNbr; i++)
	{
		m_lines[i] = new Label();
		ss << m_name << "_line" << (m_lineNbr - i - 1);
		m_lines[i]->CtrlInit(this, Vector2i(offset.x, (m_charHeight + m_gapBetLines) * i + offset.y), 
			Vector2i(), 0, ss.str());
		m_lines[i]->TextInit("", m_fontColor, m_italic, m_charHeight, m_charWidth, m_charInterval);
		m_lines[i]->Init(TEXTDOCK_NONE, Vector2f());
		ss.str("");
	}
	if (m_updownButtons)
	{
		// Init boutons
		m_upArrow = new Button();
		ss << m_name << "_uparrow";
		m_upArrow->CtrlInit(this, Vector2i(m_size.x - 30, 30), Vector2i(30,20), uptext, ss.str());
		m_upArrow->Init();
		ss.str("");
		m_downArrow = new Button();
		ss << m_name << "_downarrow";
		m_downArrow->CtrlInit(this, Vector2i(m_size.x - 30, 0), Vector2i(30,20), downtext, ss.str());
		m_downArrow->Init();
		ss.str("");
		// Events
		m_upArrow->OnClick.Attach(this, &ListBox::ScrollUp);
		m_downArrow->OnClick.Attach(this, &ListBox::ScrollDown);
	}
}

void ListBox::Render()
{
	if (m_visible)
	{
		Control::Render();
		for (unsigned short i = 0; i < m_lineNbr; i++)
			m_lines[i]->Render();
		if (m_upArrow != 0 && m_downArrow != 0)
		{
			m_upArrow->Render();
			m_downArrow->Render();
		}
	}
}

void ListBox::Update()
{
	StringList::iterator it = m_messages.begin();
	for (int i = 0; i < m_curLineIndex; i++)
	{
		if (it != m_messages.end())
			++it;
	}
	for(int i = 0; i < m_lineNbr; ++i)
	{
		if (it == m_messages.end())
			m_lines[i]->SP(PropString::PROPSTR_TEXT, "");
		else {
			m_lines[i]->SP(PropString::PROPSTR_TEXT, it->data());
			++it;
		}
	}
}

void ListBox::SetLine(unsigned short line, const std::string& message)
{
	assert(line < m_lineNbr);
	m_messages.assign(line, message);
	Update();
}

void ListBox::AddLine(const std::string& message)
{
	m_messages.push_front(message);
	Update();
}

void ListBox::Scroll(int lines)
{
	if (m_scrollable)
	{
		if (m_curLineIndex + lines < 0) {
			m_curLineIndex = 0;
			return;
		}
		if (m_curLineIndex + lines > m_messages.size() - m_lineNbr || m_messages.size() <= m_lineNbr)
			return;
		m_curLineIndex += lines;
		Update();
	}
}

void ListBox::ScrollUp(Control* sender)
{
	Scroll(1);
}

void ListBox::ScrollDown(Control* sender)
{
	Scroll(-1);
}
bool ListBox::MousePressEvents(int x, int y)
{
	return m_downArrow->MousePressEvents(x,y) ||
		m_upArrow->MousePressEvents(x,y);
}

bool ListBox::MouseReleaseEvents(int x, int y)
{
	return m_downArrow->MouseReleaseEvents(x,y) ||
		m_upArrow->MouseReleaseEvents(x,y);
}

ListBox& ListBox::operator<<( const string& text )
{
	AddLine(text);
	return *this;
}

string ListBox::GetLine( int index )
{
	StringList::iterator it = m_messages.begin();
	return it->data();
}
