#ifndef EFFECT_H_
#define EFFECT_H_

#include "define.h"
#include "engine/gl/ui/controls/workable/timer.h"

class Localizable;

class Effect
{
public:
	Effect(Localizable* control);
	virtual ~Effect();
	virtual void Animate(Timer* timer) = 0;
	virtual void TurnOn();
	virtual void TurnOff();
	virtual void Reset();

protected:
	enum StepTime
	{
		STEP_BEGIN,
		STEP_GOBACK
	};

	StepTime m_step;
	Timer m_effectTimer;
	Localizable* m_control;

	// Informations du contrôle
	Point m_c_ipos;
	Size m_c_isize;
};

#endif