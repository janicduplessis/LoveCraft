#ifndef PANEL_H_
#define PANEL_H_

#include "control.h"
#include "util/array.h"

class Panel : public Control
{
public:
	Panel();
	Panel(Control* parent, Vector2i &position, Vector2i &size, Texture* texture, const unsigned short controlnbr, const std::string& name);
	~Panel();
	void AddControl(Control* control);
	void Render();
private:
	virtual void RenderAllControls();

	(Control*)* m_controls;
	unsigned short m_capacity;
	unsigned short m_ctrlNbr;
};

#endif