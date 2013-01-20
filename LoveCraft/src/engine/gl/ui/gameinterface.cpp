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
	ss << (int)m_val->GetCharacter()->Health() << " / " << (int)m_val->GetCharacter()->HealthMax();
	m_lbl_health->SetMessage(ss.str());
	ss.str("");
	//Energie
	ss << (int)m_val->GetCharacter()->Energy() << " / " << (int)m_val->GetCharacter()->EnergyMax();
	m_lbl_energy->SetMessage(ss.str());
	ss.str("");
	//Mana
	ss << (int)m_val->GetCharacter()->Mana() << " / " << (int)m_val->GetCharacter()->ManaMax();
	m_lbl_mana->SetMessage(ss.str());
	ss.str("");
	//Experience
	ss << (int)m_val->GetCharacter()->Exp() << " / " << (int)m_val->GetCharacter()->ExpNext();
	m_lbl_exp->SetMessage(ss.str());
	ss.str("");
	//Niveau
	ss << m_val->GetCharacter()->Level();
	m_lbl_playerLevel->SetMessage(ss.str());
	ss.str("");
	//Position
	ss << "Position :     ( " << std::setprecision(4) << m_val->GetPlayer()->Position().x << ", " << std::setprecision(4) <<
		m_val->GetPlayer()->Position().y << ", " << std::setprecision(4) << m_val->GetPlayer()->Position().z << " )";
	m_lbl_plrPos->SetMessage(ss.str());
	ss.str("");
	//Vitesse
	ss << "Vitesse :      " << m_val->GetPlayer()->Speed();
	m_lbl_plrSpd->SetMessage(ss.str());
	ss.str("");
	//Accélération
	ss << "Acceleration : " << m_val->GetPlayer()->Acceleration();
	m_lbl_plrAcc->SetMessage(ss.str());
	ss.str("");
	//Psotion souris
	ss << "Pos. Souris :  " << m_val->GetMousePos();
	m_lbl_mousePos->SetMessage(ss.str());
	ss.str("");
	//FPS
	ss << "Fps :          " << std::setprecision(2) << m_val->GetFPS();
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
		//delete m_lb_infos;
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
#pragma region Controles du jeu
	// Écran
	m_pnl_screen = new Panel(0, Vector2i(), Vector2i(m_val->GetWidth(), m_val->GetHeight()), 0, 1, "main");

#pragma region Enfants de Main

	// Zone de jeu
	m_pnl_playscreen = new Panel(m_pnl_screen, 
		Vector2i(INTERFACE_SIDE_LEFT_WIDTH, INTERFACE_BOTTOM_HEIGHT),
		Vector2i(m_pnl_screen->Size().x - INTERFACE_SIDE_LEFT_WIDTH - INTERFACE_SIDE_RIGHT_WIDTH, 
		m_pnl_screen->Size().y - INTERFACE_BOTTOM_HEIGHT - INTERFACE_TOP_HEIGHT),
		0, PNL_PLAYSCREEN_CONTROLS_NBR, "playscreen");
	m_pnl_screen->AddControl(m_pnl_playscreen);

#pragma region Enfants de Playscreen

	// Informations
	m_lb_infos = new ListBox(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->Size().y - LBL_GENERIC_CHAR_H*6 - 26*12), 200, m_val->GetTexture(TEXTCOLOR_RED), 
		0, 26, 2, 12.f, 12.f, 0.5f, false, "lb_infos");
	m_pnl_playscreen->AddControl(m_lb_infos);
	m_lb_infos->AddLine("Controles Mouvements");
	m_lb_infos->AddLine("Avancer:           W");
	m_lb_infos->AddLine("Reculer:           S");
	m_lb_infos->AddLine("Droite:            D");
	m_lb_infos->AddLine("Gauche:            A");
	m_lb_infos->AddLine("Sauter:            Espace");
	m_lb_infos->AddLine("Marcher:           Ctrl");
	m_lb_infos->AddLine("Se pencher:        C");
	m_lb_infos->AddLine("Courir:            Shift");
	m_lb_infos->AddLine("");
	m_lb_infos->AddLine("Controles Debug");
	m_lb_infos->AddLine("Tirer:             1");
	m_lb_infos->AddLine("Cochon:            2");
	m_lb_infos->AddLine("Teleporter:        6");
	m_lb_infos->AddLine("Remplir Vie:       7");
	m_lb_infos->AddLine("Remplir Energie:   8");
	m_lb_infos->AddLine("Augmenter Exp:     P");
	m_lb_infos->AddLine("");
	m_lb_infos->AddLine("Options");
	m_lb_infos->AddLine("Wireframe:         Y");
	m_lb_infos->AddLine("Music On/off       O");
	m_lb_infos->AddLine("Music Next         M");
	m_lb_infos->AddLine("Aff/Cach Infos:    F9");
	//m_lb_infos->AddLine("Fullscreen         F10");
	m_lb_infos->AddLine("Quitter            Esc");

	//Fenetre de console
	m_lb_console = new ListBox(m_pnl_playscreen, 
		Vector2i(m_pnl_playscreen->Size().x - 64 - (int)LB_CONSOLE_SIZE_W, TXB_CONSOLE_SIZE_H + 5), 
		LB_CONSOLE_SIZE_W, 
		m_val->GetTexture(TEXTCOLOR_YELLOW), 
		m_val->GetTexture(CUSTIMAGE_CONSOLE_BACK), 
		LB_CONSOLE_LINE_NUMBER, 
		LB_CONSOLE_LINE_GAP, 
		LB_CONSOLE_CHAR_W, 
		LB_CONSOLE_CHAR_H, 
		LB_CONSOLE_CHAR_I, 
		LB_CONSOLE_SCROLLABLE, 
		LB_CONSOLE_NAME,
		Vector2i(LB_CONSOLE_BODER_OFFSET_S, LB_CONSOLE_BODER_OFFSET_B));
	m_pnl_playscreen->AddControl(m_lb_console);
	m_lb_console->SetRepeatTexture(false);
	Info::Get().SetConsole(m_lb_console);
	//Texbox de la console
	m_txb_console = new Textbox(m_pnl_playscreen,
		Vector2i(m_pnl_playscreen->Size().x - 64 - (int)LB_CONSOLE_SIZE_W, 0),
		Vector2i(TXB_CONSOLE_SIZE_W, TXB_CONSOLE_SIZE_H),
		m_val->GetTexture(TEXTCOLOR_WHITE),
		m_val->GetTexture(CUSTIMAGE_CONSOLE_TEXTBOX_BACK),
		Label::TEXTDOCK_MIDDLELEFT,
		LBL_GENERIC_ITALIC,
		TXB_CONSOLE_SIZE_H * 0.75f,
		TXB_CONSOLE_CHAR_I,
		Vector2f(TXB_CONSOLE_OFFSET_X, TXB_CONSOLE_OFFSET_Y),
		TXB_CONSOLE_NAME);
	m_pnl_playscreen->AddControl(m_txb_console);
	m_txb_console->SetRepeatTexture(false);
	m_txb_console->SetVisible(false);
	m_txb_console->SetFocus(false);
	// Frame portrait
	m_pnl_portrait = new Panel(m_pnl_playscreen,
		Vector2i(PNL_PORTRAIT_POSITION_X, PNL_PORTRAIT_POSITION_Y),
		Vector2i(PNL_PORTRAIT_SIZE_W, PNL_PORTRAIT_SIZE_H),
		m_val->GetTexture(CUSTIMAGE_PORTRAIT_FRAME), PNL_PORTRAIT_CONTROLS_NBR, PNL_PORTRAIT_NAME);
	m_pnl_playscreen->AddControl(m_pnl_portrait);

	m_lbl_currentBlockType = new Label(m_pnl_playscreen, Vector2i(m_pnl_portrait->Position().x,
		m_pnl_portrait->Position().y + m_pnl_portrait->Size().y + 10),
		m_val->GetTexture(TEXTCOLOR_BLUE), "Bloc : ", Label::TEXTDOCK_NONE, false, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "lblcurblock");
	m_pb_currentBlockType = new PictureBox(m_pnl_playscreen, Vector2i(m_lbl_currentBlockType->Position().x + 84, m_lbl_currentBlockType->Position().y), Vector2i(20,20), 
		m_textureArray->GetTexture(m_currentBlockType - 1), "pbcurbloc");
	m_pnl_playscreen->AddControl(m_lbl_currentBlockType);
	m_pnl_playscreen->AddControl(m_pb_currentBlockType);

#pragma region Enfants pnl portrait

	// Barre de vie
	m_pgb_health = new ProgressBar(m_pnl_portrait,
		Vector2i(PGB_HEALTH_POSITION_X, PGB_HEALTH_POSITION_Y),
		Vector2i(PGB_HEALTH_SIZE_W, PGB_HEALTH_SIZE_H),
		m_val->GetTexture(CUSTIMAGE_PGBTEXT_HEALTH), m_val->GetTexture(CUSTIMAGE_PGBTEXT_HEALTH_BACK),
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_HEALTH_BACKGROUND, PGB_HEALTH_BORDER_SIZE, PGB_HEALTH_NAME);
	m_pnl_portrait->AddControl(m_pgb_health);
	// Barre de mana
	m_pgb_mana = new ProgressBar(m_pnl_portrait,
		Vector2i(PGB_MANA_POSITION_X, PGB_MANA_POSITION_Y),
		Vector2i(PGB_MANA_SIZE_W, PGB_MANA_SIZE_H),
		m_val->GetTexture(CUSTIMAGE_PGBTEXT_MANA), m_val->GetTexture(CUSTIMAGE_PGBTEXT_MANA_BACK),
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_MANA_BACKGROUND, PGB_MANA_BORDER_SIZE, PGB_MANA_NAME);
	m_pnl_portrait->AddControl(m_pgb_mana);
	// Barre d'expérience
	m_pgb_exp = new ProgressBar(m_pnl_portrait,
		Vector2i(PGB_EXP_POSITION_X, PGB_EXP_POSITION_Y),
		Vector2i(PGB_EXP_SIZE_W, PGB_EXP_SIZE_H),
		m_val->GetTexture(CUSTIMAGE_PGBTEXT_EXP), m_val->GetTexture(CUSTIMAGE_PGBTEXT_EXP_BACK),
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_EXP_BACKGROUND, PGB_EXP_BORDER_SIZE, PGB_EXP_NAME);
	m_pnl_portrait->AddControl(m_pgb_exp);
	m_lbl_health = new Label(m_pnl_portrait, Vector2i(LBL_HEALTH_POSITION_X, LBL_HEALTH_POSITION_Y), m_val->GetTexture(TEXTCOLOR_RED), "", 
		Label::TEXTDOCK_NONE, PNL_PORTRAIT_ITALIC, PNL_PORTRAIT_CHAR_H, PNL_PORTRAIT_CHAR_W, PNL_PORTRAIT_CHAR_I, Vector2f(), LBL_HEALTH_NAME);
	m_pnl_portrait->AddControl(m_lbl_health);
	// Label de mana
	m_lbl_mana = new Label(m_pnl_portrait, Vector2i(LBL_MANA_POSITION_X, LBL_MANA_POSITION_Y), m_val->GetTexture(TEXTCOLOR_BLUE), "", 
		Label::TEXTDOCK_NONE, PNL_PORTRAIT_ITALIC, PNL_PORTRAIT_CHAR_H, PNL_PORTRAIT_CHAR_W, PNL_PORTRAIT_CHAR_I, Vector2f(), LBL_MANA_NAME);
	m_pnl_portrait->AddControl(m_lbl_mana);
	// Label d'exp
	m_lbl_exp = new Label(m_pnl_portrait, Vector2i(LBL_EXP_POSITION_X, LBL_EXP_POSITION_Y), m_val->GetTexture(TEXTCOLOR_YELLOW), "", 
		Label::TEXTDOCK_NONE, PNL_PORTRAIT_ITALIC, LBL_EXP_CHAR_W, LBL_EXP_CHAR_H, PNL_PORTRAIT_CHAR_I, Vector2f(), LBL_EXP_NAME);
	m_pnl_portrait->AddControl(m_lbl_exp);

	// Image du joueur
	m_pnl_playerImage = new Panel(m_pnl_portrait, 
		Vector2i(PB_PORTRAIT_POSITION_X, PB_PORTRAIT_POSITION_Y),
		Vector2i(PB_PORTRAIT_SIZE_W, PB_PORTRAIT_SIZE_H),
		m_val->GetTexture(CUSTIMAGE_PORTRAIT_MALE), 1, PB_PORTRAIT_NAME);
	m_pnl_portrait->AddControl(m_pnl_playerImage);

#pragma region Enfants de m_pnl_playerImage

	m_lbl_playerLevel = new Label(m_pnl_playerImage, Vector2i(), m_val->GetTexture(TEXTCOLOR_BLUE), "", 
		Label::TEXTDOCK_TOPCENTER, LBL_GENERIC_ITALIC, LBL_PLAYER_LEVEL_W, LBL_PLAYER_LEVEL_H, LBL_PLAYER_LEVEL_I, Vector2f(), LBL_PLAYER_LEVEL_NAME);
	m_pnl_playerImage->AddControl(m_lbl_playerLevel);

#pragma endregion

#pragma endregion

	//Barre d'énergie verticale
	m_pgb_energy = new ProgressBar(m_pnl_playscreen,
		Vector2i(PGB_ENERGY_POSITION_X, PGB_ENERGY_POSITION_Y),
		Vector2i(PGB_ENERGY_SIZE_W, PGB_ENERGY_SIZE_H),
		m_val->GetTexture(CUSTIMAGE_PGBTEXT_ENERGY), m_val->GetTexture(CUSTIMAGE_PGBTEXT_ENERGY_BACK),
		ProgressBar::BARMODE_VERTICAL_DTU, PGB_ENERGY_BACKGROUND, PGB_ENERGY_BORDER_SIZE, PGB_ENERGY_NAME);
	m_pnl_playscreen->AddControl(m_pgb_energy);
	//Label d'énergie
	m_lbl_energy = new Label(m_pnl_playscreen, Vector2i(LBL_ENERGY_POSITION_X, LBL_ENERGY_POSITION_Y), m_val->GetTexture(TEXTCOLOR_GREEN), "", 
		Label::TEXTDOCK_NONE, LBL_ENERGY_ITALIC, LBL_ENERGY_CHAR_H, LBL_ENERGY_CHAR_W, LBL_ENERGY_CHAR_I, Vector2f(), LBL_ENERGY_NAME);
	m_pnl_playscreen->AddControl(m_lbl_energy);

#pragma region Controles de debug

	//Label Position
	m_lbl_plrPos = new Label(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->Size().y - LBL_GENERIC_CHAR_H), m_val->GetTexture(TEXTCOLOR_GREEN), "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "pos");
	m_pnl_playscreen->AddControl(m_lbl_plrPos);
	//Label Vitesse
	m_lbl_plrSpd = new Label(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->Size().y - LBL_GENERIC_CHAR_H*2), m_val->GetTexture(TEXTCOLOR_BLUE), "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "spd");
	m_pnl_playscreen->AddControl(m_lbl_plrSpd);
	//Label Acceleration
	m_lbl_plrAcc = new Label(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->Size().y - LBL_GENERIC_CHAR_H*3), m_val->GetTexture(TEXTCOLOR_RED), "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "acc");
	m_pnl_playscreen->AddControl(m_lbl_plrAcc);
	//Label mouse position
	m_lbl_mousePos = new Label(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->Size().y - LBL_GENERIC_CHAR_H*4), m_val->GetTexture(TEXTCOLOR_WHITE), "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "Mpos");
	m_pnl_playscreen->AddControl(m_lbl_mousePos);
	//Label FPS
	m_lbl_FPS = new Label(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->Size().y - LBL_GENERIC_CHAR_H*5), m_val->GetTexture(TEXTCOLOR_YELLOW), "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "fps");
	m_pnl_playscreen->AddControl(m_lbl_FPS);

#pragma endregion

	//Heure
	m_pnl_time = new Panel(m_pnl_playscreen, Vector2i(m_pnl_playscreen->Size().x - 128, m_pnl_playscreen->Size().y - 64), 
		Vector2i(128, 64), m_val->GetTexture(CUSTIMAGE_CLOCK_BG), 1, "clock");
	m_pnl_playscreen->AddControl(m_pnl_time);

#pragma region Enfants de m_pnl_time

	m_lbl_time = new Label(m_pnl_time, Vector2i(0,0), m_val->GetTexture(TEXTCOLOR_WHITE), "", Label::TEXTDOCK_MIDDLECENTER, false, 
		LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "time");
	m_pnl_time->AddControl(m_lbl_time);

#pragma endregion

#pragma endregion

#pragma endregion

#pragma endregion
}