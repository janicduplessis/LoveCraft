#ifndef BUTTON_H__
#define BUTTON_H__

#include "../singletext.h"

class Button : public SingleText
{
public:
	Button();
	virtual ~Button();
	virtual void Render();

	/**
	 * Évenement On Click trigger si le bouton est cliqué
	 * @param pointeur vers le bouton
	 */
	Event<void, Control*> OnClick;
	/**
	 * Test si le bouton est cliqué
	 * Trigger le event OnClick et la metode Press
	 * 
	 * @param x position de la souris en x
	 * @param y position de la souris en y
	 * @return si l'event a été géré
	 */
	virtual bool MousePressEvents(int x, int y);
	/**
	 * Relache le bouton
	 */
	virtual bool MouseReleaseEvents(int x, int y);
	/**
	 * Pèse sur le bouton
	 */
	void Press();

	virtual bool IsHeld() const;
protected:
	Button(CONTROLTYPE type);
	bool m_clicked;
};

#endif