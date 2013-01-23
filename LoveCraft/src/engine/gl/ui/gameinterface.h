#ifndef GAMEINTERFACE_H_
#define GAMEINTERFACE_H_

#include "interface.h"
#include "valuesgameinterface.h"

#include <string>

class GameInterface : public Interface
{
public:
	GameInterface();
	~GameInterface();

	virtual void Init(const ValuesGameInterface& val);
	virtual void Update(const ValuesGameInterface& val);
	virtual void Render();

	virtual void ConsoleWriteLine(const std::string& line);

	virtual bool MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
	virtual bool MouseRleaseEvent(const MOUSE_BUTTON &button, int x, int y);
	virtual bool KeyPressEvent(unsigned char key);
	virtual bool KeyReleaseEvent(unsigned char key);
	virtual bool TextenteredEvent(unsigned int val);

private:
	virtual void TextUpdate(const ValuesGameInterface& val);

public:
	Panel* m_pnl_screen;
	Panel* m_pnl_playscreen;
	Panel* m_pnl_portrait;
	Panel* m_pnl_welcome;

	ProgressBar* m_pgb_health;
	ProgressBar* m_pgb_energy;
	ProgressBar* m_pgb_mana;
	ProgressBar* m_pgb_exp;

	ListBox* m_lb_infos;
	ListBox* m_lb_console;
	Textbox* m_txb_console;

	Panel* m_pnl_playerImage;
	Label* m_lbl_playerLevel;

	Label* m_lbl_plrPos;
	Label* m_lbl_plrSpd;
	Label* m_lbl_plrAcc;
	Label* m_lbl_mousePos;
	Label* m_lbl_FPS;

	Panel* m_pnl_time;
	Label* m_lbl_time;

	Label* m_lbl_health;
	Label* m_lbl_mana;
	Label* m_lbl_exp;
	Label* m_lbl_energy;

	Label* m_lbl_currentBlockType;
	PictureBox* m_pb_currentBlockType;
	PictureBox* m_pb_welcomeface;
	Label* m_lbl_welcomemessage;
};

#endif