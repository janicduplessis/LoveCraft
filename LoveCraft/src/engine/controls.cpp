#include "controls.h"



void Controls::Reset()
{
	m_controls.Reset(false);
}

void Controls::Set( unsigned int index, bool value )
{
	m_controls.Set(index, value);
}

bool Controls::Get( unsigned int index ) const
{
	return m_controls.Get(index);
}

Controls::~Controls()
{

}

Controls::Controls() : m_controls(Array<bool>(256, false))
{

}

bool Controls::Q() const
{
	//NYI
	return false;
}

bool Controls::W() const
{
	return m_controls.Get(22);
}

bool Controls::E() const
{
	//NYI
	return false;
}

bool Controls::R() const
{
	//NYI
	return false;
}

bool Controls::T() const
{
	//NYI
	return false;
}

bool Controls::Y() const
{
	//NYI
	return false;
}

bool Controls::U() const
{
	//NYI
	return false;
}

bool Controls::I() const
{
	//NYI
	return false;
}

bool Controls::O() const
{
	//NYI
	return false;
}

bool Controls::P() const
{
	//NYI
	return false;
}

bool Controls::A() const
{
	//NYI
	return false;
}

bool Controls::S() const
{
	return m_controls.Get(18);
}

bool Controls::D() const
{
	return m_controls.Get(3);
}

bool Controls::F() const
{
	//NYI
	return false;
}

bool Controls::G() const
{
	//NYI
	return false;
}

bool Controls::H() const
{
	//NYI
	return false;
}

bool Controls::J() const
{
	//NYI
	return false;
}

bool Controls::K() const
{
	//NYI
	return false;
}

bool Controls::L() const
{
	//NYI
	return false;
}

bool Controls::Z() const
{
	//NYI
	return false;
}

bool Controls::X() const
{
	//NYI
	return false;
}

bool Controls::C() const
{
	//NYI
	return false;
}

bool Controls::V() const
{
	//NYI
	return false;
}

bool Controls::B() const
{
	//NYI
	return false;
}

bool Controls::N() const
{
	//NYI
	return false;
}

bool Controls::M() const
{
	//NYI
	return false;
}

bool Controls::n1() const
{
	//NYI
	return false;
}

bool Controls::n2() const
{
	//NYI
	return false;
}

bool Controls::n3() const
{
	//NYI
	return false;
}

bool Controls::n4() const
{
	//NYI
	return false;
}

bool Controls::n5() const
{
	//NYI
	return false;
}

bool Controls::n6() const
{
	//NYI
	return false;
}

bool Controls::n7() const
{
	//NYI
	return false;
}

bool Controls::n8() const
{
	//NYI
	return false;
}

bool Controls::n9() const
{
	//NYI
	return false;
}

bool Controls::n0() const
{
	//NYI
	return false;
}

bool Controls::Shift() const
{
	return m_controls.Get(38);
}

bool Controls::Ctrl() const
{
	//NYI
	return false;
}

bool Controls::Alt() const
{
	//NYI
	return false;
}

bool Controls::Space() const
{
	//NYI
	return false;
}
