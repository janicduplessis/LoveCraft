#ifndef ITEXT_H__
#define ITEXT_H__

#include "../texture.h"

class IText
{
public:
	IText();
	virtual ~IText();
	virtual void TextInit(const string& message, bool italic, float charHeight, float charWidth, float charinterval);
protected:
	string m_message;
	bool m_italic;
	float m_charWidth;
	float m_charHeight;
	float m_charInterval;
};

#endif