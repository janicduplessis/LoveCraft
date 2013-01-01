#ifndef PANEL_H_
#define PANEL_H_

#include "control.h"
#include "util/array.h"

class Panel : public Control
{
public:
	Panel();
	Panel(Vector2i parent, Vector2i &position, Vector2i &size, Texture* texture, const unsigned short controlnbr, const std::string& name);
	~Panel();
	//void AddControl(const Control& control);
	//Control& GetControlById(unsigned short index) const;
	//Control& GetControlByName(const std::string& name) const;
	void Render();
	void RenderAllControls();
private:
	void Resize(unsigned short newNbr);

	//Control* m_controls;
	unsigned short m_controlNbr;
	unsigned short m_index;
};

#endif