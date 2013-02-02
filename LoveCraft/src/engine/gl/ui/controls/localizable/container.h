#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "define.h"
#include "util/vector2.h"
#include "../localizable.h"
#include <cassert>
#include <string>
#include <iostream>

class Container : public Localizable
{
public:
	Container(CONTROLTYPE type);
	virtual ~Container();
	virtual void Init(uint8 capacity);
	virtual void Render();
	virtual void AddControl(Localizable* control);
	virtual Localizable* GetControlById(uint8 index) const;
	virtual Localizable* GetControlByName(const string& name) const;
protected:
	Localizable** m_controls;
	uint8 m_ctrlNbr;
	uint8 m_capacity;
};

#endif