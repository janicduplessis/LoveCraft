#include "debuginterface.h"


DebugInterface::DebugInterface() : Interface()
{
}


DebugInterface::~DebugInterface()
{
	DeInit();
}

void DebugInterface::Init(const ValuesInterface& val)
{
	// controles de test
	m_timertest = new Panel();
	m_timertest->InitControl("timertest");
	m_timertest->InitLocalizable(Point(100, 100), Size(400, 400), CUSTIMAGE_CONSOLE_BACK, 0);

	m_timertesttext = new Label();
	m_timertesttext->InitControl("text");
	m_timertesttext->InitLocalizable(Point(100, 300), m_timertest);
	m_timertesttext->SetCharHeight(24.f);
	m_timertesttext->SetCharWidth(14.f);
	m_timertesttext->SetCharInterval(0.5f);
	m_timertesttext->SetColor(TEXTCOLOR_BLUE);
	m_timertesttext->SetMsg("Appuyez sur espace pour tester.");

	m_timertesttime = new Label();
	m_timertesttime->InitControl("text");
	m_timertesttime->InitLocalizable(Point(100, 300), m_timertest);
	m_timertesttime->SetCharHeight(24.f);
	m_timertesttime->SetCharWidth(14.f);
	m_timertesttime->SetCharInterval(0.5f);
	m_timertesttime->SetOffset(Point(0, 70));
	m_timertesttime->SetDocking(TEXTDOCK_MIDDLECENTER);
	m_timertesttime->SetColor(TEXTCOLOR_BLUE);
	m_timertesttime->SetMsg("Interval du label: &var");


	m_loaded = true;
}

void DebugInterface::DeInit()
{
	if (IsLoaded())
	{
		delete m_timertest;
		delete m_timertesttext;
		delete m_timertesttime;
	}
	m_loaded = false;
}

void DebugInterface::Render()
{
	m_timertest->Render();
}

void DebugInterface::Show()
{
	m_timertest->Show();
}

void DebugInterface::Hide()
{
	m_timertest->Hide();
}
