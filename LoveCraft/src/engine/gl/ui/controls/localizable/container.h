#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "../localizable.h"

class Container : public Localizable
{
public:
	Container(CONTROLTYPE type);
	virtual ~Container();
	virtual void InitContainer(uint8 capacity);
	virtual void Render() = 0;
	virtual void AddControl(Localizable* control);
	virtual Localizable* GetControlById(uint8 index) const;
	virtual Localizable* GetControlByName(const string& name) const;
protected:
	Localizable** m_controls;
	uint8 m_ctrlNbr;
	uint8 m_capacity;
};

#endif