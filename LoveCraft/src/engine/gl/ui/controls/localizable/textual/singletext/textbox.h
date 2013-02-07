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

	virtual void SetMaxLength(uint16 maxlength);
	virtual uint16 GetMaxLength() const;
	virtual bool IsMaxLength(uint16 maxlength) const;
	virtual void AddMaxLength(uint16 value);

protected:
	bool m_hasFocus;
	uint16 m_maxLength;
};

#endif