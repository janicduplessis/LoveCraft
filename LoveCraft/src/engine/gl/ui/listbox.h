#ifndef LISTBOX_H_
#define LISTBOX_H_

#include "control.h"
#include "label.h"
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
	void Scroll(int lines);
	void AddLine(const std::string& message);
	ListBox& operator<<(const std::string& text);
	void Update();
private:
	(Label*)* m_lines;
	typedef std::list<std::string> StringList;
	StringList m_messages;
	Texture* m_fontMainColor;
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