#ifndef TEXTBOX_H_
#define TEXTBOX_H_

#include "../singletext.h"

class Textbox : public SingleText
{
public:
	Textbox();
	virtual ~Textbox();
	virtual void Render();

	Event<void, Textbox*> LostFocus;
	Event<void, Textbox*> GainedFocus;

	virtual void Update(unsigned int val);
	virtual void GiveFocus();
	virtual void RemoveFocus();
	virtual bool HasFocus() const;

protected:
	bool m_hasFocus;
};

#endif