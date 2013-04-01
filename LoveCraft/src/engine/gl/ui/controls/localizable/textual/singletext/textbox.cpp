#include "textbox.h"
#include "../label.h"

Textbox::Textbox() : SingleText(CTRLTYPE_TEXTBOX), m_hasFocus(false), m_maxLength(64)
{
}

Textbox::~Textbox()
{
}

void Textbox::Render()
{
	if (IsVisible())
	{
		DrawSquare();
		Text->Render();
	}
}

#pragma region Class functions

void Textbox::Update(unsigned int val)
{
	static std::ostringstream ss;

	//Appuie sur Enter pour acquérir le focus
	if (val == 13 && !HasFocus())
	{
		ResetMsg();
		GiveFocus();
		Show();
		return;
	}

	//Capture du texte en mode Textbox
	if (HasFocus())
	{
		//Vérification qu'il s'agit d'un caractère valide
		if (val >= 32 && val <= 126 && GetMsg().length() <= m_maxLength)
		{
			ss << static_cast<char>(val);
			AddMsg(ss.str());
			ss.str("");
			return;
		}
		//Si c'est un backspace
		if (val == 8)
		{
			string mes = GetMsg();
			if (mes.length() > 0)
			{
				for (int i = 0; i < mes.length()-1; i++)
					ss << mes[i];
				SetMsg(ss.str());
				ss.str("");
			}
		}
		//Si c'est un return
		if (val == 13)
		{
			Hide();
			RemoveFocus();
		}
	}
}

#pragma endregion

// Propriétés

#pragma region Focus

void Textbox::GiveFocus()
{
	if (m_hasFocus)
		return;
	m_hasFocus = true;
	GainedFocus.Notify(this);
}
void Textbox::RemoveFocus()
{
	if (!m_hasFocus)
		return;
	m_hasFocus = false;
	LostFocus.Notify(this);
}
bool Textbox::HasFocus() const
{
	return m_hasFocus;
}

#pragma endregion

#pragma region Max length

void Textbox::SetMaxLength(uint16 maxlength)
{
	if (m_maxLength == maxlength)
		return;
	m_maxLength = maxlength;
}
uint16 Textbox::GetMaxLength() const
{
	return m_maxLength;
}
bool Textbox::IsMaxLength(uint16 maxlength) const
{
	return m_maxLength == maxlength;
}
void Textbox::AddMaxLength(int16 value)
{
	int newValue = value;
	if ((int)m_maxLength + value < 0)
		newValue = 0;
	SetMaxLength(GetMaxLength() + newValue);
}

#pragma endregion