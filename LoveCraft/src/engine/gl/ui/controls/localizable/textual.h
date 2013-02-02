#ifndef TEXTUAL_H__
#define TEXTUAL_H__

#include "../localizable.h"

class Textual : public Localizable
{
public:
	Textual(CONTROLTYPE type);
	virtual ~Textual();
	virtual void Init(Texture* color, bool italic, float charHeight, float charWidth, float charinterval, Texture* background = 0);
	virtual void Init(Theme* theme);

	virtual void Render() = 0;

	virtual void SetMsg(string message);
	virtual string GetMsg() const;
	virtual bool IsMsg(string message) const;

	virtual void SetColor(Texture* color);
	virtual Texture* GetColor() const;
	virtual bool IsColor(Texture* color) const;

	virtual void SetItalic();
	virtual void RemoveItalic();
	virtual bool IsItalic() const;

	virtual void SetCharHeight(float height);
	virtual float GetCharHeight() const;
	virtual float IsCharHeight(float height) const;

	virtual void SetCharWidth(float width);
	virtual float GetCharWidh() const;
	virtual float IsCharWidth(float width) const;
	
	virtual void SetCharInterval(float interval);
	virtual float GetCharInterval() const;
	virtual float IsCharInterval(float interval) const;
protected:
	string m_message;
	Texture* m_fontColor;
	bool m_italic;
	float m_charWidth;
	float m_charHeight;
	float m_charInterval;
};

#endif