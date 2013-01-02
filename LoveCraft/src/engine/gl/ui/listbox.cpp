#include "listbox.h"
#include <sstream>

ListBox::ListBox() : Control(CTRLTYPE_LISTBOX), m_lines(0), m_fontMainColor(0), m_lineNbr(0), m_gapBetLines(2), m_charWidth(12.f),
	m_charHeight(12.f), m_charInterval(0.66f)
{

}

ListBox::ListBox(Control* parent, Vector2i &position, Vector2i &size, Texture* textMainColor, Texture* background, unsigned short linenbr,
				 short linegap, float charwidth, float charheight, float charinterval, const std::string& name) : 
Control(CTRLTYPE_LISTBOX, parent, position, size, background, name), 
	m_fontMainColor(textMainColor), m_lineNbr(linenbr), m_gapBetLines(linegap), m_charWidth(charwidth), m_charHeight(charheight), m_charInterval(charinterval)
{
	std::ostringstream ss;
	m_lines = new Label[m_lineNbr];
	for (unsigned short i = 0; i < m_lineNbr; i++)
	{
		ss << m_name << "_line" << (m_lineNbr - i - 1);
		m_lines[m_lineNbr - i - 1] = Label(this, 
			Vector2i(0, (m_charHeight + m_gapBetLines) * i), 
			Vector2i(), 
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
}

ListBox::~ListBox()
{
	delete [] m_lines;
}

void ListBox::Render()
{
	Control::Render();
	for (unsigned short i = 0; i < m_lineNbr; i++)
		m_lines[i].Render();
}

void ListBox::SetLine(unsigned short line, std::string message)
{
	assert(line < m_lineNbr);
	m_lines[line].SetMessage(message);
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
	m_texture = l.m_texture;
	m_type = l.m_type;
	m_visible = l.m_visible;

	return *this;
}