#ifndef LISTBOX_H_
#define LISTBOX_H_

#include "control.h"
#include "label.h"
#include <list>

class ListBox : public Control
{
public:
	ListBox();
	ListBox(Control* parent, Vector2i &position, float lineWidth, Texture* textMainColor, Texture* background, unsigned short linenbr,
		short linegap, float charwidth, float charheight, float charinterval, const std::string& name);
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
	unsigned int m_curLineIndex;
};

#endif