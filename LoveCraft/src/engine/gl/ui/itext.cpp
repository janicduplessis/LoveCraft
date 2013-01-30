#include "itext.h"


IText::IText() : Structure(), m_message(""), m_italic(false), m_charHeight(0.f), 
	m_charWidth(0.f), m_charInterval(0.f), m_fontColor(0)
{
}


IText::~IText()
{
}

void IText::TextInit(string message, Texture* color, bool italic, float charHeight, float charWidth, float charinterval)
{
	if (!m_initialized)
	{
		m_message = message;
		m_fontColor = color;
		m_italic = italic;
		m_charHeight = charHeight;
		m_charWidth = charWidth;
		m_charInterval = charinterval;
		Structure::Init();
	}
}