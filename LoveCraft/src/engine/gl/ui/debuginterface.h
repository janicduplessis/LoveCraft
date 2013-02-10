#ifndef DEBUGINTERFACE_H_
#define DEBUGINTERFACE_H_

#include "interface.h"

class DebugInterface : Interface
{
public:
	DebugInterface();
	virtual ~DebugInterface();
	virtual void Init(const ValuesInterface& val);
	virtual void DeInit();
	virtual void Render();
	virtual void Show();
	virtual void Hide();
	
	Panel* m_timertest;
	Label* m_timertesttext;
	Label* m_timertesttime;

	
};

#endif