#include "debuginterface.h"


DebugInterface::DebugInterface()
{
}


DebugInterface::~DebugInterface()
{
}

void DebugInterface::Init(const ValuesInterface& val)
{
	// controles de test
	m_timertest = new Panel();
	m_timertest->InitControl("timertest");
	m_timertest->InitLocalizable(Point(100, 100), Size(400, 400), val.UITextures[CUSTIMAGE_CONSOLE_BACK], 0);

	m_timertesttext = new Label();
	m_timertesttext->InitControl("text");
	m_timertesttext->InitLocalizable(Point(100, 300), m_timertest);
	m_timertesttext->SetCharHeight(24.f);
	m_timertesttext->SetCharWidth(14.f);
	m_timertesttext->SetCharInterval(0.5f);
	m_timertesttext->SetColor(val.FontTextures[TEXTCOLOR_BLUE]);
	m_timertesttext->SetMsg("Appuyez sur espace pour tester.");
	m_timertest->AddControl(m_timertesttext);

	m_timertesttime = new Label();
	m_timertesttime->InitControl("text");
	m_timertesttime->InitLocalizable(Point(100, 300), m_timertest);
	m_timertesttime->SetCharHeight(24.f);
	m_timertesttime->SetCharWidth(14.f);
	m_timertesttime->SetCharInterval(0.5f);
	m_timertesttime->SetOffset(Point(0, 70));
	m_timertesttime->SetDocking(TEXTDOCK_MIDDLECENTER);
	m_timertesttime->SetColor(val.FontTextures[TEXTCOLOR_BLUE]);
	m_timertesttime->SetMsg("Interval du label: &var");
	m_timertest->AddControl(m_timertesttime);
}

void DebugInterface::Render()
{
	m_timertest->Render();
}