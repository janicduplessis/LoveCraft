#ifndef SPELLBUTTON_H__
#define SPELLBUTTON_H__

#include "button.h"

class SpellButton : Button
{
public:
	SpellButton();
	virtual ~SpellButton();

	virtual void SetAlternate(Texture* image);
	virtual Texture* Getalternate() const;
	virtual bool IsAlternate(Texture* image) const;
	virtual void ResetAlternate();

	virtual void ShowAlternate();
	virtual void HideAlternate();
protected:
	virtual void DrawingBindTexture();
	Texture* m_alternateImage;
	bool m_showalternate;
};

#endif