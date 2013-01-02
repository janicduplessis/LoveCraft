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
	virtual IContainer& operator=(const IContainer& p);
	virtual Vector2i AbsolutePosition() = 0;
	virtual Vector2i Size() = 0;
protected:
	void RenderAllControls();
	(Control*)* m_controls;
	unsigned short m_capacity;
	unsigned short m_ctrlNbr;
};

#endif