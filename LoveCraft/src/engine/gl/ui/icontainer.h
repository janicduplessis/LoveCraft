#ifndef ICONTAINER_H_
#define ICONTAINER_H_

#include "define.h"
#include "../texture.h"
#include "util/vector2.h"
#include "control.h"
#include <cassert>
#include <string>
#include <iostream>

class IContainer
{
public:
	IContainer(unsigned short capacity);
	virtual ~IContainer();
	virtual void AddControl(Control* control);
	virtual Control* GetControlById(unsigned short index) const;
	virtual Control* GetControlByName(const std::string& name) const;
protected:
	void RenderAllControls();
	Control** m_controls;
	unsigned short m_capacity;
	unsigned short m_ctrlNbr;
};

#endif