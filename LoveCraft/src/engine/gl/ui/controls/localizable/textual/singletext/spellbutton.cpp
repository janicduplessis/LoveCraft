#include "spellbutton.h"
#include "../label.h"

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

// Propriétés

#pragma region Show alternate

void SpellButton::ShowAlternate()
{
	if (m_showalternate)
		return;
	m_showalternate = true;
}
void SpellButton::HideAlternate()
{
	if (!m_showalternate)
		return;
	m_showalternate = false;
}

#pragma endregion

#pragma region Alternate

void SpellButton::SetAlternate(Texture* image)
{
	if (m_alternateImage == image)
		return;
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
	if (m_alternateImage == m_background)
		return;
	m_alternateImage = m_background;
}

#pragma endregion