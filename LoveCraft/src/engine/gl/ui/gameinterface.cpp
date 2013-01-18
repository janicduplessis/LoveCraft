#include "gameinterface.h"
#include <iostream>
#include <iomanip>
#include <sstream>


GameInterface::GameInterface() : Interface()
{
	
}

void GameInterface::TextUpdate()
{
	std::ostringstream ss;
	//Vie
	ss << (int)m_engine->m_character.Health() << " / " << (int)m_engine->m_character.HealthMax();
	m_lbl_health->SetMessage(ss.str());
	ss.str("");
	//Energie
	ss << (int)m_engine->m_character.Energy() << " / " << (int)m_engine->m_character.EnergyMax();
	m_lbl_energy->SetMessage(ss.str());
	ss.str("");
	//Mana
	ss << (int)m_engine->m_character.Mana() << " / " << (int)m_engine->m_character.ManaMax();
	m_lbl_mana->SetMessage(ss.str());
	ss.str("");
	//Experience
	ss << (int)m_engine->m_character.Exp() << " / " << (int)m_engine->m_character.ExpNext();
	m_lbl_exp->SetMessage(ss.str());
	ss.str("");
	//Niveau
	ss << m_engine->m_character.Level();
	m_lbl_playerLevel->SetMessage(ss.str());
	ss.str("");
	//Position
	ss << "Position :     ( " << std::setprecision(4) << m_engine->m_player->Position().x << ", " << std::setprecision(4) <<
		m_engine->m_player->Position().y << ", " << std::setprecision(4) << m_engine->m_player->Position().z << " )";
	m_lbl_plrPos->SetMessage(ss.str());
	ss.str("");
	//Vitesse
	ss << "Vitesse :      " << m_engine->m_player->Speed();
	m_lbl_plrSpd->SetMessage(ss.str());
	ss.str("");
	//Acc�l�ration
	ss << "Acceleration : " << m_engine->m_player->Acceleration();
	m_lbl_plrAcc->SetMessage(ss.str());
	ss.str("");
	//Psotion souris
	ss << "Pos. Souris :  " << "( " << m_engine->MousePosition().x << ", " << m_engine->MousePosition().y << " )";
	m_lbl_mousePos->SetMessage(ss.str());
	ss.str("");
	//FPS
	ss << "Fps :          " << std::setprecision(2) << m_engine->m_fps;
	m_lbl_FPS->SetMessage(ss.str());
	ss.str("");
	//Heure
	time_t currentTime;
	time (&currentTime);
	struct tm ptm;
	localtime_s(&ptm, &currentTime);
	ss << std::setfill('0');
	ss << std::setw(2) << ptm.tm_hour << ":" << std::setw(2) << ptm.tm_min;
	m_lbl_time->SetMessage(ss.str());
	ss.str("");
}

GameInterface::~GameInterface()
{
	if (m_loaded)
	{
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
		delete m_lbl_currentBlockType;
		delete m_pb_currentBlockType;
	}
}

void GameInterface::InitControls()
{

}