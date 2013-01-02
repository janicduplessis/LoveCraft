#ifndef LISTBOX_H_
#define LISTBOX_H_

#include "control.h"
#include "label.h"

class ListBox : public Control
{
public:
	ListBox();
	ListBox(Control* parent, Vector2i &position, Vector2i &size, Texture* textMainColor, Texture* background, unsigned short linenbr,
		short linegap, float charwidth, float charheight, float charinterval, const std::string& name);
	~ListBox();
	void SetLine(unsigned short line, std::string message);
	void Render();
	ListBox& operator=(const ListBox& p);
private:
	Label* m_lines;
	Texture* m_fontMainColor;
	unsigned short m_lineNbr;
	short m_gapBetLines;
	float m_charWidth;
	float m_charHeight;
	float m_charInterval;
};

#endif