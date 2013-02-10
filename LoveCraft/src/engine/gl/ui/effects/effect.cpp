#include "effect.h"
#include "engine/gl/ui/controls/localizable.h"

Effect::Effect(Localizable* control) : m_step(STEP_BEGIN), m_effectTimer(Timer()), m_control(control)
{
	m_effectTimer.Init(0.2f);
	m_effectTimer.OnTick.Attach(this, &Effect::Animate);
	m_effectTimer.Stop();
	m_c_ipos = control->GetPosition();
	m_c_isize = control->GetSize();
}


Effect::~Effect()
{
}

void Effect::TurnOn()
{
	m_effectTimer.Start();
}
void Effect::TurnOff()
{
	m_effectTimer.Stop();
	Reset();
}
void Effect::Reset()
{
	m_control->SetPosition(m_c_ipos);
	m_control->SetSize(m_c_isize);
	m_step = STEP_BEGIN;
}