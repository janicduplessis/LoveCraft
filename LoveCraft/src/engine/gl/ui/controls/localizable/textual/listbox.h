#ifndef LISTBOX_H_
#define LISTBOX_H_

#include "../textual.h"
#include <list>

class Button;
class Label;

class ListBox : public Textual
{
public:
	ListBox();
	virtual ~ListBox();
	virtual void InitListBox(uint8 lineNbr, int8 gap, Point offset, bool scrollable);
	virtual void InitListBox(uint8 lineNbr, int8 gap, Point offset, bool scrollable, IMAGE uptext, IMAGE downtext);
	virtual void SetLine(unsigned short line, const string& message);
	virtual void Render();

	virtual void SetSize(Size size);

	virtual void MakeScrollable();
	virtual void MakeUnscrollable();
	virtual bool IsScrollable() const;

	virtual void ShowScrollButtons();
	virtual void HideScrollButtons();
	virtual bool IsScrollButtons();

	virtual void SetGap(uint8 gap);
	virtual uint8 GetGap() const;
	virtual bool IsGap(uint8 gap) const;

	virtual void SetOffset(Point offset);
	virtual Point GetOffset() const;
	virtual bool IsOffset(Point offset) const;

	virtual void SetWidth(uint16 width);
	virtual uint16 GetWidth() const;
	virtual bool IsWidth(uint16 width) const;

	virtual void SetFontColor(COLOR color);
	virtual COLOR GetFontColor() const;
	virtual bool IsFontColor(COLOR color) const;
	virtual bool GetFontColorTexture() const;

	virtual void AddLine(const string& message);
	virtual string GetLine(int index);
	virtual void Scroll(int lines);
	virtual void ScrollUp(Control* sender);
	virtual void ScrollDown(Control* sender);
	virtual bool MousePressEvents(int x, int y);
	virtual bool MouseReleaseEvents(int x, int y);
protected:
	virtual void Update();
	virtual void UpdateValues();
	typedef std::list<string> StringList;
	Label** m_lines;
	StringList m_messages;
	Point m_lineOffset;
	uint16 m_width;
	bool m_showButtons;
	Button* m_upArrow;
	Button* m_downArrow;
	unsigned short m_lineNbr;
	short m_gapBetLines;
	bool m_scrollable;
	unsigned int m_curLineIndex;
};

#endif