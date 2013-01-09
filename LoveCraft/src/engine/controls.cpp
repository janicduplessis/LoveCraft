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
	return m_controls.Get(16);
}

bool Controls::W() const
{
	return m_controls.Get(22);
}

bool Controls::E() const
{
	return m_controls.Get(4);
}

bool Controls::R() const
{
	return m_controls.Get(17);
}

bool Controls::T() const
{
	return m_controls.Get(19);
}

bool Controls::Y() const
{
	return m_controls.Get(24);
}

bool Controls::U() const
{
	return m_controls.Get(20);
}

bool Controls::I() const
{
	return m_controls.Get(8);
}

bool Controls::O() const
{
	return m_controls.Get(14);
}

bool Controls::P() const
{
	return m_controls.Get(15);
}

bool Controls::A() const
{
	return m_controls.Get(0);
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
	return m_controls.Get(5);
}

bool Controls::G() const
{
	return m_controls.Get(6);
}

bool Controls::H() const
{
	return m_controls.Get(7);
}

bool Controls::J() const
{
	return m_controls.Get(9);
}

bool Controls::K() const
{
	return m_controls.Get(10);
}

bool Controls::L() const
{
	return m_controls.Get(11);
}

bool Controls::Z() const
{
	return m_controls.Get(25);
}

bool Controls::X() const
{
	return m_controls.Get(23);
}

bool Controls::C() const
{
	return m_controls.Get(2);
}

bool Controls::V() const
{
	return m_controls.Get(21);
}

bool Controls::B() const
{
	return m_controls.Get(1);
}

bool Controls::N() const
{
	return m_controls.Get(13);
}

bool Controls::M() const
{
	return m_controls.Get(12);
}

bool Controls::n1() const
{
	return m_controls.Get(27);
}

bool Controls::n2() const
{
	return m_controls.Get(28);
}

bool Controls::n3() const
{
	return m_controls.Get(29);
}

bool Controls::n4() const
{
	return m_controls.Get(30);
}

bool Controls::n5() const
{
	return m_controls.Get(31);
}

bool Controls::n6() const
{
	return m_controls.Get(32);
}

bool Controls::n7() const
{
	return m_controls.Get(33);
}

bool Controls::n8() const
{
	return m_controls.Get(34);
}

bool Controls::n9() const
{
	return m_controls.Get(35);
}

bool Controls::n0() const
{
	return m_controls.Get(26);
}

bool Controls::f1() const
{
	return m_controls.Get(85);
}
bool Controls::f2() const
{
	return m_controls.Get(86);
}
bool Controls::f3() const
{
	return m_controls.Get(87);
}
bool Controls::f4() const
{
	return m_controls.Get(88);
}
bool Controls::f5() const
{
	return m_controls.Get(89);
}
bool Controls::f6() const
{
	return m_controls.Get(90);
}
bool Controls::f7() const
{
	return m_controls.Get(91);
}
bool Controls::f8() const
{
	return m_controls.Get(92);
}
bool Controls::f9() const
{
	return m_controls.Get(93);
}
bool Controls::f10() const
{
	return m_controls.Get(94);
}
bool Controls::f11() const
{
	return m_controls.Get(95);
}
bool Controls::f12() const
{
	return m_controls.Get(96);
}

bool Controls::Shift() const
{
	return m_controls.Get(38);
}

bool Controls::Ctrl() const
{
	return m_controls.Get(37);
}

bool Controls::Alt() const
{
	return m_controls.Get(39);
}

bool Controls::Space() const
{
	return m_controls.Get(57);
}

bool Controls::BackSpace() const
{
	return m_controls.Get(59);
}
bool Controls::Enter() const
{
	return m_controls.Get(58);
}
bool Controls::Esc() const
{
	return m_controls.Get(36);
}