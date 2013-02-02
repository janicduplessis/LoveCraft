#ifndef TEXTBOX_H_
#define TEXTBOX_H_

#include "label.h"

class Textbox : public Textual
{
public:
	Textbox();
	virtual ~Textbox();
	void Init(Vector2i &offset = Vector2i());
	bool HasFocus() const;
	void SetFocus(const bool value);
	void SetMessage(string text);
	string GetMsg() const;
	Label* TLabel();
	void Render();
private:
	Label* m_label;
	bool m_hasFocus;
};

#endif