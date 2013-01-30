#include "itext.h"


IText::IText() : m_message("message"), m_italic(false), m_charHeight(0.f), m_charWidth(0.f), m_charInterval(0.f)
{
}


IText::~IText()
{
}

void IText::TextInit(const string& message, bool italic, float charHeight, float charWidth, float charinterval)
{
	m_message = message;
	m_italic = italic;
	m_charHeight = charHeight;
	m_charWidth = charWidth;
	m_charInterval = charinterval;
}