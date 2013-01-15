#include "gameinterface.h"


GameInterface::GameInterface() : Interface()
{
}


GameInterface::~GameInterface()
{

#ifdef GAME_INTERFACE_INITIALIZED

	delete m_pnl_screen;
	delete m_pnl_playscreen;
	delete m_pnl_portrait;
	delete m_pgb_health;
	delete m_pgb_energy;
	delete m_pgb_mana;
	delete m_pgb_exp;
	delete m_lb_infos;
	delete m_lb_console;
	delete m_txb_console;
	delete m_pnl_playerImage;
	delete m_lbl_playerLevel;
	delete m_lbl_plrPos;
	delete m_lbl_plrSpd;
	delete m_lbl_plrAcc;
	delete m_lbl_FPS;
	delete m_pnl_time;
	delete m_lbl_time;
	delete m_lbl_health;
	delete m_lbl_mana;
	delete m_lbl_exp;
	delete m_lbl_energy;

#endif

}

void GameInterface::Init()
{

}