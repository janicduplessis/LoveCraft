#ifndef TEXTUAL_H__
#define TEXTUAL_H__

#include "../localizable.h"

class Textual : public Localizable
{
public:
	Textual(CONTROLTYPE type);
	virtual ~Textual();
	virtual void InitTextual(COLOR color, bool italic, float charHeight, float charWidth, float charinterval, IMAGE background = CUSTIMAGE_NONE);

	virtual void Render() = 0;

	virtual void SetMsg(string message);
	virtual void AddMsg(string value);
	virtual string GetMsg() const;
	virtual bool IsMsg(string message) const;
	virtual bool HasMsg() const;
	virtual void ResetMsg();

	virtual void SetColor(COLOR color);
	virtual COLOR GetColor() const;
	virtual bool IsColor(COLOR color) const;
	virtual Texture* GetColorTexture() const;

	virtual void SetItalic();
	virtual void RemoveItalic();
	virtual bool IsItalic() const;

	virtual void SetCharHeight(float height);
	virtual void AddCharHeight(float value);
	virtual float GetCharHeight() const;
	virtual float IsCharHeight(float height) const;

	virtual void SetCharWidth(float width);
	virtual void AddCharWidth(float value);
	virtual float GetCharWidh() const;
	virtual float IsCharWidth(float width) const;

	virtual void SetCharInterval(float interval);
	virtual void AddCharInterval(float value);
	virtual float GetCharInterval() const;
	virtual float IsCharInterval(float interval) const;
protected:
	string m_message;
	COLOR m_fontColor;
	bool m_italic;
	float m_charWidth;
	float m_charHeight;
	float m_charInterval;
};

#endif