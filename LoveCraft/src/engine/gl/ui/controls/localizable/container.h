#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "../localizable.h"

class Container : public Localizable
{
public:
	Container(CONTROLTYPE type);
	virtual ~Container();
	virtual void Render() = 0;

	virtual void AddControl(Localizable* control);
	virtual Localizable* GetControlById(uint8 index) const;
	virtual Localizable* GetControlByName(const string& name) const;
	virtual Localizable* GetTopControl(int x, int y);

	virtual uint8 GetControlNumber() const;
	virtual bool IsControlNomber(uint8 number) const;
protected:
	Localizable** m_controls;
	uint8 m_ctrlNbr;
};

#endif