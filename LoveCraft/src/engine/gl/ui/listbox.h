#ifndef LISTBOX_H_
#define LISTBOX_H_

#include "control.h"
#include "label.h"
#include "button.h"
#include <list>

class ListBox : public Control, public IText
{
public:
	ListBox();
	~ListBox();
	void Init(unsigned short lineNbr, short gap, Vector2i offset, bool scrollable);
	void Init(unsigned short lineNbr, short gap, Vector2i offset, bool scrollable, Texture* uptext, Texture* downtext);
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