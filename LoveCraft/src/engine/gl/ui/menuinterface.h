#ifndef MENUINTERFACE_H_
#define MENUINTERFACE_H_

#include "interface.h"

class MenuInterface : public Interface
{
public:
	MenuInterface();
	virtual ~MenuInterface();
	virtual void Init(const ValuesInterface& val);
	virtual void Render();
	virtual void DeInit();

	//Controles

	Panel* pnl_screen;
	Panel* pnl_main;
	PictureBox* pb_logo;
	Panel* pnl_controls;
	Button* btn_normStart;
	Button* btn_debugStart;
	Button* btn_close;

	Panel* pnl_loading;
};

#endif