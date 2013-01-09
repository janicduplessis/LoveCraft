#ifndef TEXTBOX_H_
#define TEXTBOX_H_

#include "control.h"
#include "label.h"

class Textbox : public Control
{
public:
	Textbox(Control* parent, Vector2i position, Vector2i size, Texture* color, Texture* back, Label::Docking dock, bool italic, float charWidth, float charinterval, Vector2f offset, const std::string& name);
	~Textbox();
	bool HasFocus() const;
	void SetFocus(const bool value);
	void SetMessage(std::string text);
	std::string GetMsg() const;
	void Render();
private:
	Label* m_label;
	bool m_hasFocus;
};

#endif