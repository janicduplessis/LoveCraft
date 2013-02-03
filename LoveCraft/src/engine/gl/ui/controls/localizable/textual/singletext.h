#ifndef SINGLETEXT_H__
#define SINGLETEXT_H__

#include "../textual.h"
#include "label.h"

class SingleText : public Textual
{
public:
	SingleText(CONTROLTYPE type);
	virtual ~SingleText();
	virtual void InitTextual(Texture* color, bool italic, float charHeight, float charWidth, float charinterval, Texture* background = 0);
	virtual void InitTextual(ThemeSet theme);
	virtual void Render() = 0;
	virtual void SetMsg(string message);
	virtual void SetColor(Texture* color);
	virtual void SetItalic();
	virtual void RemoveItalic();
	virtual void SetCharHeight(float height);
	virtual void SetCharWidth(float width);
	virtual void SetCharInterval(float interval);
	virtual void SetOffset(Point offset);
	virtual void SetDocking(Docking dock);
	virtual Docking GetDocking() const;
	virtual bool IsDocking(Docking dock) const;
	virtual void UseNextDocking();
protected:
	Label* m_label;
};

#endif