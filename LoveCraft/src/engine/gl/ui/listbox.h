#ifndef LISTBOX_H_
#define LISTBOX_H_

#include "control.h"
#include "label.h"
#include "button.h"
#include <list>

class ListBox : public Control
{
public:
	ListBox();
	ListBox(Control* parent, const Vector2i& position, float lineWidth, Texture* textMainColor, Texture* background, unsigned short linenbr,
		short linegap, float charwidth, float charheight, float charinterval, bool scrollable, const std::string& name, const Vector2i& offset = Vector2i());
	~ListBox();
	void Init(unsigned short lineNbr, short gap, bool scrollable, bool updownbuttons = false);
	void SetLine(unsigned short line, const std::string& message);
	void Render();
	void AddLine(const std::string& message);
	std::string GetLine(int index);
	ListBox& operator<<(const std::string& text);
	void Update();
	void Scroll(int lines);
	void ScrollUp(Control* sender);
	void ScrollDown(Control* sender);
	virtual bool MousePressEvents(int x, int y);
	virtual bool MouseReleaseEvents(int x, int y);
private:
	Label** m_lines;
	typedef std::list<std::string> StringList;
	StringList m_messages;
	bool m_updownButtons;
	Button* m_upArrow;
	Button* m_downArrow;
	unsigned short m_lineNbr;
	short m_gapBetLines;
	bool m_scrollable;
	unsigned int m_curLineIndex;
};

#endif