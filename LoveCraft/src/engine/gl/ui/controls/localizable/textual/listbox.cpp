#include "listbox.h"

ListBox::ListBox() : Textual(CTRLTYPE_LISTBOX), m_lines(0), m_lineNbr(0), m_gapBetLines(0), 
	m_curLineIndex(0), m_scrollable(false), m_upArrow(0), m_downArrow(0), m_showButtons(false)
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

void ListBox::InitListBox(uint8 lineNbr, int8 gap, Point offset, bool scrollable)
{
	InitListBox(lineNbr, gap, offset, scrollable, 0, 0);
}

void ListBox::InitListBox(uint8 lineNbr, int8 gap, Point offset, bool scrollable, Texture* uptext, Texture* downtext)
{
	m_lineNbr = lineNbr;
	m_gapBetLines = gap;
	m_scrollable = scrollable;
	m_showButtons = uptext != 0 || downtext != 0;


	SetSize(Size(GetSize().w + offset.x * 2, (GetCharHeight() + gap) * lineNbr + offset.y * 2 - gap));

	m_lines = new Label*[lineNbr];
	std::ostringstream ss;
	for (uint8 i = 0; i < m_lineNbr; i++)
	{
		m_lines[i] = new Label();
		ss << GetName() << "_line" << (m_lineNbr - i - 1);
		m_lines[i]->InitControl(ss.str(), this);
		m_lines[i]->InitLocalizable(Point(offset.x, (GetCharHeight() + m_gapBetLines) * i + offset.y), Size(), 0);
		m_lines[i]->InitTextual(GetColor(), IsItalic(), GetCharHeight(), GetCharWidh(), GetCharInterval());
		ss.str("");
	}
	if (m_showButtons)
	{
		// Init boutons
		m_upArrow = new Button();
		ss << m_name << "_uparrow";
		m_upArrow->InitControl(ss.str(), this);
		m_upArrow->InitLocalizable(Point(GetSize().w - 30, 30), Size(30, 20), 0);
		ss.str("");
		m_downArrow = new Button();
		ss << m_name << "_downarrow";
		m_downArrow->InitControl(ss.str(), this);
		m_downArrow->InitLocalizable(Point(GetSize().w - 30, 0), Size(30, 20), 0);
		ss.str("");
		// Events
		m_upArrow->OnClick.Attach(this, &ListBox::ScrollUp);
		m_downArrow->OnClick.Attach(this, &ListBox::ScrollDown);
	}
}

void ListBox::UpdateValues()
{
	SetSize(Size(GetSize().w + m_lineOffset.x * 2, 
		(m_charHeight + m_gapBetLines) * m_lineNbr + m_lineOffset.y * 2 - m_gapBetLines));
	for (uint8 i = 0; i < m_lineNbr; i++)
	{
		m_lines[i]->SetPosition(Point(m_lineOffset.x, (GetCharHeight() + m_gapBetLines) * i + m_lineOffset.y));
		m_lines[i]->SetColor(m_fontColor);
		m_lines[i]->SetCharHeight(GetCharHeight());
		m_lines[i]->SetCharWidth(GetCharWidh());
		m_lines[i]->SetCharInterval(GetCharInterval());
		if (IsItalic())
			m_lines[i]->SetItalic();
		else m_lines[i]->RemoveItalic();
	}
}

void ListBox::Render()
{
	if (m_visible)
	{
		DrawSquare();
		for (uint8 i = 0; i < m_lineNbr; i++)
			m_lines[i]->Render();
		if (m_showButtons)
		{
			if (m_upArrow)
				m_upArrow->Render();
			if (m_downArrow)
				m_downArrow->Render();
		}
	}
}

void ListBox::SetSize(Size size)
{
	Localizable::SetSize(size);
	UpdateValues();
}

void ListBox::MakeScrollable()
{
	m_scrollable = true;
}
void ListBox::MakeUnscrollable()
{
	m_scrollable = false;
}
bool ListBox::IsScrollable() const
{
	return m_scrollable;
}

void ListBox::ShowScrollButtons()
{
	m_showButtons = true;
}
void ListBox::HideScrollButtons()
{
	m_showButtons = false;
}
bool ListBox::IsScrollButtons()
{
	return m_showButtons;
}

void ListBox::SetGap(uint8 gap)
{
	m_gapBetLines = gap;
	UpdateValues();
}
uint8 ListBox::GetGap() const
{
	return m_gapBetLines;
}
bool ListBox::IsGap(uint8 gap) const
{
	return m_gapBetLines == gap;
}

void ListBox::SetOffset(Point offset)
{
	m_lineOffset = offset;
	UpdateValues();
}
Point ListBox::GetOffset() const
{
	return m_lineOffset;
}
bool ListBox::IsOffset(Point offset) const
{
	return m_lineOffset == offset;
}

void ListBox::SetFontColor(Texture* color)
{
	m_fontColor = color;
	UpdateValues();
}
Texture* ListBox::GetFontColor() const
{
	return m_fontColor;
}
bool ListBox::IsFontColor(Texture* color) const
{
	return m_fontColor == color;
}

void ListBox::Update()
{
	StringList::iterator it = m_messages.begin();
	for (uint16 i = 0; i < m_curLineIndex; i++)
	{
		if (it != m_messages.end())
			++it;
	}
	for(uint16 i = 0; i < m_lineNbr; ++i)
	{
		if (it == m_messages.end())
			m_lines[i]->SetMsg("");
		else {
			m_lines[i]->SetMsg(it->data());
			++it;
		}
	}
}

void ListBox::SetLine(unsigned short line, const string& message)
{
	assert(line < m_lineNbr);
	m_messages.assign(line, message);
	Update();
}

void ListBox::AddLine(const string& message)
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

string ListBox::GetLine( int index )
{
	StringList::iterator it = m_messages.begin();
	return it->data();
}
