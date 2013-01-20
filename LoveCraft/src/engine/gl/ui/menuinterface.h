#ifndef MENUINTERFACE_H_
#define MENUINTERFACE_H_

#include "interface.h"

class MenuInterface : public Interface
{
public:
	MenuInterface();
	~MenuInterface();
	virtual void InitControls();
	void Render();

	//Controles

	Panel* m_menu_screen;
	PictureBox* m_menu_loading;
	Panel* m_menu_panel;
	PictureBox* m_menu_logo;
	Panel* m_menu_controls;
	Button* m_menu_start;
	Button* m_menu_fullscreen;
	Button* m_menu_close;
};

#endif