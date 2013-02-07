#include "timer.h"


Timer::Timer() : Workable(CTRLTYPE_TIMER), m_interval(1.0f), m_laps(0), m_time(0)
{
	Disable();
}


Timer::~Timer()
{
}

void Timer::Init(float intervaltime)
{
	m_interval = intervaltime > 0 ? intervaltime : 0;
}

void Timer::Update(float elapsedtime)
{
	if (IsEnabled())
	{
		if (m_time >= m_interval)
		{
			OnTick.Notify(this);
			m_time = 0;
			m_laps++;
			return;
		}
		m_time += elapsedtime;
	}
}

void Timer::Start()
{
	Enable();
}
uint16 Timer::Stop()
{
	uint16 laps = m_laps;
	Disable();
	m_laps = 0;
	return laps;
}
void Timer::Reset()
{
	m_laps = 0;
}

void Timer::SetIntervalTime(float intervaltime)
{
	m_interval = intervaltime;
}
void Timer::AddIntervalTime(float value)
{
	m_interval += value;
}
float Timer::GetIntervalTime() const
{
	return m_interval;
}
bool Timer::IsIntervalTime(float intervaltime)
{
	return fabs(m_interval) - fabs(intervaltime) <= 0.0001f;
}

uint16 Timer::GetLaps() const
{
	return m_laps;
}
float Timer::GetTime() const
{
	return m_time;
}