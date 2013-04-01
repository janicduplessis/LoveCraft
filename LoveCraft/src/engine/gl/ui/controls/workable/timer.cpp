#include "timer.h"


Timer::Timer() : Workable(CTRLTYPE_TIMER), m_interval(1000), m_laps(0), m_time(0), m_intervalf(1.0f)
{
	Disable();
}

Timer::~Timer()
{
}

#pragma region Class funtions

void Timer::Init(uint16 intervaltime)
{
	SetIntervalTime(intervaltime);
}

void Timer::Update(float elapsedtime)
{
	if (IsEnabled())
	{
		if (m_time >= m_intervalf)
		{
			OnTick.Notify(this);
			m_time = 0;
			m_laps++;
			return;
		}
		m_time += elapsedtime;
	}
}

#pragma endregion

// Propriétés

#pragma region State

void Timer::Start()
{
	if (IsEnabled())
		return;
	Enable();
}
uint16 Timer::Stop()
{
	if (!IsEnabled())
		return 0;

	uint16 laps = m_laps;
	Disable();
	m_laps = 0;
	return laps;
}
void Timer::Reset()
{
	m_laps = 0;
}

#pragma endregion

#pragma region Enabled

void Timer::Enable()
{
	Control::Enable();
}
void Timer::Disable()
{
	Control::Disable();
}

#pragma endregion

#pragma region Intervaltime

void Timer::SetIntervalTime(int16 intervaltime)
{
	if (m_interval == intervaltime)
		return;
	m_interval = intervaltime > 0 ? intervaltime : 0;
	m_intervalf = (float)m_interval / 1000;
}
void Timer::AddIntervalTime(int16 value)
{
	SetIntervalTime((int16)GetIntervalTime() + value);
}
uint16 Timer::GetIntervalTime() const
{
	return m_interval;
}
bool Timer::IsIntervalTime(uint16 intervaltime)
{
	return m_interval == intervaltime;
}

#pragma endregion

#pragma region Laps

uint16 Timer::GetLaps() const
{
	return m_laps;
}

#pragma endregion

#pragma region Time

float Timer::GetTime() const
{
	return m_time;
}

#pragma endregion