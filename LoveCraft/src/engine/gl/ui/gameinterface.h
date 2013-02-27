#ifndef GAMEINTERFACE_H_
#define GAMEINTERFACE_H_

#include "interface.h"
#include "valuesgameinterface.h"

#include <string>

class GameInterface : public Interface
{
public:
	GameInterface();
	virtual ~GameInterface();

	virtual void Init(const ValuesGameInterface& val);
	virtual void Update(const ValuesGameInterface& val);
	virtual void Render();
	virtual void DeInit();

	virtual void Show();
	virtual void Hide();

	virtual void ConsoleWriteLine(const string& line);

	virtual bool MouseMoveEvents(int x, int y);
	virtual bool MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
	virtual bool MouseRleaseEvent(const MOUSE_BUTTON &button, int x, int y);
	virtual bool KeyPressEvent(unsigned char key);
	virtual bool KeyReleaseEvent(unsigned char key);
	virtual bool TextenteredEvent(unsigned int val);

private:
	virtual void TextUpdate(const ValuesGameInterface& val);

public:
	Panel* pnl_screen;
	Panel* pnl_playscreen;
	Panel* pnl_portrait;

	ProgressBar* pgb_health;
	ProgressBar* pgb_energy;
	ProgressBar* pgb_mana;
	ProgressBar* pgb_exp;

	ListBox* lbx_infos;
	ListBox* lbx_console;
	Textbox* txb_console;

	PictureBox* pnl_playerImage;
	Label* lbl_playerLevel;

	Label* lbl_plrPos;
	Label* lbl_plrSpd;
	Label* lbl_plrAcc;
	Label* lbl_mousePos;
	Label* lbl_FPS;
	Label* m_controlmousetest;

	Panel* pnl_time;
	Label* lbl_time;

	Label* lbl_plrHealth;
	Label* lbl_plrMana;
	Label* lbl_plrExp;
	Label* lbl_plrEnergy;

	Label* lbl_crntBT;
	PictureBox* pb_crntBT;

	PictureBox* pb_cursor;
};

#endif