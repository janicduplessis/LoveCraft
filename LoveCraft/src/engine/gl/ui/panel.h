#ifndef PANEL_H_
#define PANEL_H_

#include "control.h"
#include "icontainer.h"

class Panel : public Control, public IContainer
{
public:
	Panel();
	~Panel();
	void Init();
	// Avant l'héritage multiple
	//void AddControl(Control* control);
	void Render();
	// Avant l'héritage multiple
	//private:
	//virtual void RenderAllControls();

	//(Control*)* m_controls;
	//unsigned short m_capacity;
	//unsigned short m_ctrlNbr;
};

#endif