#ifndef DEBUGINTERFACE_H_
#define DEBUGINTERFACE_H_

#include "interface.h"

class DebugInterface
{
public:
	DebugInterface();
	~DebugInterface();
	virtual void Init(const ValuesInterface& val);
	void Render();

	Panel* m_timertest;
	Label* m_timertesttext;
	Label* m_timertesttime;
};

#endif