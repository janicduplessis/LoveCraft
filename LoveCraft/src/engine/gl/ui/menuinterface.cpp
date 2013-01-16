#include "menuinterface.h"


MenuInterface::MenuInterface() : Interface()
{
}


MenuInterface::~MenuInterface()
{
	delete m_menu_close;
	delete m_menu_controls;
	delete m_menu_fullscreen;
	delete m_menu_logo;
	delete m_menu_panel;
	delete m_menu_screen;
	delete m_menu_start;
}

void MenuInterface::Init()
{

}