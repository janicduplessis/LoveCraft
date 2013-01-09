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
	void SetLine(unsigned short line, const std::string& message);
	void Render();
	ListBox& operator=(const ListBox& p);
	void AddLine(const std::string& message);
	ListBox& operator<<(const std::string& text);
	void Update();
	void Scroll(int lines);
	void ScrollUp(Control* sender);
	void ScrollDown(Control* sender);
	void MouseClick(int x, int y);
	void MouseRelease();
private:
	(Label*)* m_lines;
	typedef std::list<std::string> StringList;
	StringList m_messages;
	Texture* m_fontMainColor;
	Texture* m_texDownArrow;
	Texture* m_texUpArrow;
	Button* m_upArrow;
	Button* m_downArrow;
	unsigned short m_lineNbr;
	short m_gapBetLines;
	float m_charWidth;
	float m_charHeight;
	float m_charInterval;
	bool m_scrollable;
	Vector2i m_offset;
	unsigned int m_curLineIndex;
};

#endif