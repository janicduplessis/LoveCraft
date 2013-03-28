#ifndef SPELLBUTTON_H__
#define SPELLBUTTON_H__

#include "button.h"

class PictureBox;
class Label;

class SpellButton : Button
{
public:
	SpellButton();
	virtual ~SpellButton();
	virtual void Init(const uint8 spellNbr);
	virtual void Render();
	
	virtual void Activate();
	virtual void Desactivate();
	virtual bool IsActive() const;
protected:
	PictureBox* m_offIcon;
	uint8 m_spellNbr;
	bool m_active;
};

#endif