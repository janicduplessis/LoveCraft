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
	virtual void Show();
	virtual void Hide();
	virtual bool MouseMoveEvents(const MouseEventArgs& e);
	virtual bool MousePressEvents(const MOUSE_BUTTON &button, int x, int y);
	virtual bool MouseReleaseEvents(const MOUSE_BUTTON &button, int x, int y);

	//Controles

	Panel* pnl_screen;
	Panel* pnl_main;
	PictureBox* pb_logo;
	Panel* pnl_controls;
	Button* btn_normStart;
	Button* btn_debugStart;
	Button* btn_close;

	Panel* pnl_loading;
	PictureBox* pb_cursor;

	Label* m_mousepostest;
	Label* m_controlmousetest;
};

#endif