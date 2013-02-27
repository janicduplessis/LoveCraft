#ifndef SINGLETEXT_H__
#define SINGLETEXT_H__

#include "../textual.h"

class Label;

class SingleText : public Textual
{
public:
	SingleText(CONTROLTYPE type);
	virtual ~SingleText();
	virtual void InitTextual(COLOR color, bool italic, float charHeight, float charWidth, float charinterval);
	virtual void InitTextual(ThemeSet theme);
	virtual void Render() = 0;

	virtual void SetMsg(string message);
	virtual void AddMsg(string value);
	virtual void ResetMsg();
	virtual void SetColor(COLOR color);
	virtual void SetItalic();
	virtual void RemoveItalic();
	virtual void SetCharHeight(float height);
	virtual void AddCharHeight(float value);
	virtual void SetCharWidth(float width);
	virtual void AddCharWidth(float value);
	virtual void SetCharInterval(float interval);
	virtual void AddCharInterval(float value);
	virtual void SetOffset(Point offset);
	virtual void SetDocking(Docking dock);
	virtual Docking GetDocking() const;
	virtual bool IsDocking(Docking dock) const;
	virtual void UseNextDocking();
protected:
	Label* m_label;
};

#endif