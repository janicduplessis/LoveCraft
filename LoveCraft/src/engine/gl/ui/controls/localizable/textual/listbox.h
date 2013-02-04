#ifndef LISTBOX_H_
#define LISTBOX_H_

#include "../textual.h"
#include "label.h"
#include "singletext/button.h"
#include <list>

class ListBox : public Textual
{
public:
	ListBox();
	virtual ~ListBox();
	virtual void InitListBox(uint8 lineNbr, int8 gap, Point offset, bool scrollable);
	virtual void InitListBox(uint8 lineNbr, int8 gap, Point offset, bool scrollable, Texture* uptext, Texture* downtext);
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

	virtual void SetFontColor(Texture* color);
	virtual Texture* GetFontColor() const;
	virtual bool IsFontColor(Texture* color) const;

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
	bool m_showButtons;
	Button* m_upArrow;
	Button* m_downArrow;
	unsigned short m_lineNbr;
	short m_gapBetLines;
	bool m_scrollable;
	unsigned int m_curLineIndex;
};

#endif