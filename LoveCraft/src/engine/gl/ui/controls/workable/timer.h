#ifndef TIMER_H__
#define TIMER_H__

#include "../workable.h"

class Timer : public Workable
{
public:
	Timer();
	virtual ~Timer();
	virtual void Init(float intervaltime);

	Event<void, Timer*> OnTick;

	virtual void Update(float elapsedtime);

	virtual void Start();
	virtual uint16 Stop();
	virtual void Reset();

	virtual void SetIntervaltime(float intervaltime);
	virtual float GetIntervalTime() const;
	virtual bool IsIntervalTime(float intervaltime);

	virtual uint16 GetLaps() const;
	virtual float GetTime() const;
protected:
	float m_interval;
	uint16 m_laps;
	float m_time;
};

#endif