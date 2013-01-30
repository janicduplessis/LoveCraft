#ifndef ITEXT_H__
#define ITEXT_H__

#include "../texture.h"
#include "structure.h"

class IText : public Structure
{
public:
	IText();
	virtual ~IText();
	virtual void TextInit(const string& message, Texture* color, bool italic, float charHeight, float charWidth, float charinterval);
protected:
	string m_message;
	Texture* m_fontColor;
	bool m_italic;
	float m_charWidth;
	float m_charHeight;
	float m_charInterval;
};

#endif