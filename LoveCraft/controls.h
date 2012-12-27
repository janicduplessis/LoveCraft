#ifndef CONTROLS_H__
#define CONTROLS_H__

#include "arraybool.h"

class Controls 
{
public:
	Controls();
	~Controls();

	bool Get(unsigned int index) const;
	void Set(unsigned int index, bool value);
	void Reset();

	bool Q() const;
	bool W() const;
	bool E() const;
	bool R() const;
	bool T() const;
	bool Y() const;
	bool U() const;
	bool I() const;
	bool O() const;
	bool P() const;

	bool A() const;
	bool S() const;
	bool D() const;
	bool F() const;
	bool G() const;
	bool H() const;
	bool J() const;
	bool K() const;
	bool L() const;

	bool Z() const;
	bool X() const;
	bool C() const;
	bool V() const;
	bool B() const;
	bool N() const;
	bool M() const;

	bool n1() const;
	bool n2() const;
	bool n3() const;
	bool n4() const;
	bool n5() const;
	bool n6() const;
	bool n7() const;
	bool n8() const;
	bool n9() const;
	bool n0() const;

	bool Shift() const;
	bool Ctrl() const;
	bool Alt() const;
	bool Space() const;

private:
	Array<bool> m_controls;
};


#endif