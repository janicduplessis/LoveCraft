#ifndef BUTTON_H__
#define BUTTON_H__

#include "control.h"
#include "label.h"

/**
 * Class bouton
 */
class Button : public Control, public IText
{
public:
	Button();
	~Button();
	void Init(string &text, Texture* fontColor);

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

	void SetTextTo(string text);
	string GetText() const;

	Label* TLabel();

	/**
	 * Relache le bouton
	 */
	virtual bool MouseReleaseEvents(int x, int y);

	/**
	 * Pèse sur le bouton
	 */
	void Press();

	void Render();

private:
	bool m_clicked;
	Label* m_label;
};

#endif