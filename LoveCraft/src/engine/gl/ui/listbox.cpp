#include "listbox.h"
#include <sstream>
#include "../../info.h"

ListBox::ListBox() : Control(CTRLTYPE_LISTBOX), m_lines(0), m_fontMainColor(0), m_lineNbr(0), m_gapBetLines(2), m_charWidth(12.f),
	m_charHeight(12.f), m_charInterval(0.66f), m_curLineIndex(0), m_scrollable(false)
{

}

ListBox::ListBox(Control* parent, const Vector2i &position, float lineWidth, Texture* textMainColor, Texture* background, unsigned short linenbr,
				 short linegap, float charwidth, float charheight, float charinterval, bool scrollable, const std::string& name, const Vector2i& offset) : 
Control(CTRLTYPE_LISTBOX, parent, position, Vector2i(lineWidth + offset.x * 2, (charheight + linegap) * linenbr + offset.y * 2), background, name), 
	m_fontMainColor(textMainColor), m_lineNbr(linenbr), m_gapBetLines(linegap), m_charWidth(charwidth), m_charHeight(charheight), m_charInterval(charinterval),
	m_curLineIndex(0), m_scrollable(scrollable), m_offset(offset), m_upArrow(0), m_downArrow(0), m_texUpArrow(0), m_texDownArrow(0)
{
	m_lines = new Label*[linenbr];
	std::ostringstream ss;
	for (unsigned short i = 0; i < m_lineNbr; i++)
	{
		ss << m_name << "_line" << (m_lineNbr - i - 1);
		m_lines[i] = new Label(this, 
			Vector2i(m_offset.x, (m_charHeight + m_gapBetLines) * i + m_offset.y), 
			m_fontMainColor,
			"",
			Label::TEXTDOCK_NONE,
			false,
			m_charHeight,
			m_charWidth,
			m_charInterval,
			Vector2f(),
			ss.str());
		ss.str("");
	}
	if (m_scrollable) {
		// Texture arrows
		m_texDownArrow = new Texture();
		m_texDownArrow->Load(TEXTURE_PATH "i_arrowbutton_down.jpg");
		m_texUpArrow = new Texture();
		m_texUpArrow->Load(TEXTURE_PATH "i_arrowbutton_up.jpg");
		// Init boutons
		m_upArrow = new Button(this, Vector2i(m_size.x - 30, 30), Vector2i(30,20), m_texUpArrow, m_fontMainColor, "", "bup");
		m_downArrow = new Button(this, Vector2i(m_size.x - 30, 0), Vector2i(30,20), m_texDownArrow, m_fontMainColor, "", "bdown");
		// Events
		m_upArrow->OnClick.Attach(this, &ListBox::ScrollUp);
		m_downArrow->OnClick.Attach(this, &ListBox::ScrollDown);
	}

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
	delete m_texDownArrow;
	delete m_texUpArrow;
}

void ListBox::Render()
{
	if (m_visible)
	{
		Control::Render();
		for (unsigned short i = 0; i < m_lineNbr; i++)
			m_lines[i]->Render();
		if (m_scrollable) {
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
			m_lines[i]->SetMessage("");
		else {
			m_lines[i]->SetMessage(it->data());
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

ListBox& ListBox::operator=(const ListBox& l)
{
	m_charInterval = l.m_charInterval;
	m_lines = l.m_lines;
	m_lineNbr = l.m_lineNbr;
	m_charHeight = l.m_charHeight;
	m_charWidth = l.m_charWidth;
	m_charInterval = l.m_charInterval;
	m_fontMainColor = l.m_fontMainColor;
	m_gapBetLines = l.m_gapBetLines;
	m_name = l.m_name;
	m_parent = l.m_parent;
	m_pngBlend = l.m_pngBlend;
	m_position = l.m_position;
	m_size = l.m_size;
	m_type = l.m_type;
	m_visible = l.m_visible;
	m_texture = l.m_texture;

	return *this;
}

ListBox& ListBox::operator<<( const std::string& text )
{
	AddLine(text);
	return *this;
}

std::string ListBox::GetLine( int index )
{
	StringList::iterator it = m_messages.begin();
	return it->data();
}
