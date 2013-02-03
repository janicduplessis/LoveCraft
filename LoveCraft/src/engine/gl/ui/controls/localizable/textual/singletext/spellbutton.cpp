#include "spellbutton.h"


SpellButton::SpellButton() : Button(CTRLTYPE_SPELLBUTTON), m_alternateImage(0), m_showalternate(false)
{
	m_alternateImage = m_background;
	m_label->SetDocking(TEXTDOCK_TOPRIGHT);
}


SpellButton::~SpellButton()
{
}

void SpellButton::DrawingBindTexture()
{
	if (m_showalternate && m_alternateImage)
		m_alternateImage->Bind();
	else if (m_background) 
		m_background->Bind();
}

void SpellButton::ShowAlternate()
{
	m_showalternate = true;
}
void SpellButton::HideAlternate()
{
	m_showalternate = false;
}

#pragma region Alternate

void SpellButton::SetAlternate(Texture* image)
{
	m_alternateImage = image;
}
Texture* SpellButton::Getalternate() const
{
	return m_alternateImage;
}
bool SpellButton::IsAlternate(Texture* image) const
{
	return m_alternateImage == image;
}
void SpellButton::ResetAlternate()
{
	m_alternateImage = m_background;
}

#pragma endregion