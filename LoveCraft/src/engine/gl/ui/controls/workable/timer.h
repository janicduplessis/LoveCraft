#ifndef TIMER_H__
#define TIMER_H__

#include "../workable.h"
#include "SFML/System.hpp"

class Timer : public Workable
{
public:
	Timer();
	virtual ~Timer();
	virtual void Init(uint16 intervaltime);

	Event<void, Timer*> OnTick;

	virtual void Update(float elapsedtime);

	virtual void Start();
	virtual uint16 Stop();
	virtual void Reset();

	virtual void SetIntervalTime(int16 intervaltime);
	virtual void AddIntervalTime(int16 value);
	virtual uint16 GetIntervalTime() const;
	virtual bool IsIntervalTime(uint16 intervaltime);

	virtual uint16 GetLaps() const;
	virtual float GetTime() const;

protected:
	virtual void Enable();
	virtual void Disable();

protected:
	uint16 m_interval;
	float m_intervalf;
	uint16 m_laps;
	float m_time;
};

#endif