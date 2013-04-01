#include "spellbutton.h"
#include "../label.h"
#include "../../drawable/picturebox.h"

SpellButton::SpellButton() : Button(CTRLTYPE_SPELLBUTTON), m_offIcon(0), m_active(false), m_spellNbr(0)
{
}

SpellButton::~SpellButton()
{
}

void SpellButton::Init(const uint8 spellNbr)
{
	m_spellNbr = spellNbr;

	m_offIcon = new PictureBox();
	m_offIcon->InitControl("offimage");
	m_offIcon->InitLocalizable(GetPosition(), GetSize(), CUSTIMAGE_SPELL_OFFLINE, GetParent());

	Button::InitTextual(TEXTCOLOR_GREEN, false, 12.0f, 12.0f, 0.6f);
	Text->SetDocking(TEXTDOCK_TOPRIGHT);
}

void SpellButton::Render()
{
	Button::Render();

	if (m_active)
		m_offIcon->Render();
}

// Propriétés

#pragma region Active

void SpellButton::Activate()
{
	if (m_active)
		return;
	m_active = true;
}
void SpellButton::Desactivate()
{
	if (!m_active)
		return;
	m_active = false;
}
bool SpellButton::IsActive() const
{
	return m_active;
}

#pragma endregion