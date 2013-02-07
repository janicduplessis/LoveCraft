#include "textbox.h"

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
		m_label->Render();
	}
}

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

#pragma region Focus

void Textbox::GiveFocus()
{
	m_hasFocus = true;
	GainedFocus.Notify(this);
}
void Textbox::RemoveFocus()
{
	m_hasFocus = false;
	LostFocus.Notify(this);
}
bool Textbox::HasFocus() const
{
	return m_hasFocus;
}

#pragma endregion

#pragma region MaxLength

void Textbox::SetMaxLength(uint16 maxlength)
{
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
void Textbox::AddMaxLength(uint16 value)
{
	if ((int)m_maxLength + value < 0)
		return;
	m_maxLength += value;
}

#pragma endregion