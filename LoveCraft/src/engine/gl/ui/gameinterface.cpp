#include "gameinterface.h"
#include <iostream>
#include <iomanip>
#include <sstream>


GameInterface::GameInterface() : Interface()
{

}

void GameInterface::Update(const ValuesGameInterface& val)
{
#pragma region Proprietes des controles

	// Update le picturebox du current blocktype
	m_pb_currentBlockType->SP(PROPTEXT_BACKGROUND, val.BlocTextures->GetTexture(
		Info::Get().GetBlocInfo(val.CurrentBlocType)->GetTextureIndex(0)));
	//Affiche ou cache les infos s'il y a un changement
	if (m_lb_infos->GP(PROPBOL_VISIBLE) && !Info::Get().Options().GetOptInfos())
		m_lb_infos->SP(PROPBOL_VISIBLE, false);
	else if (!m_lb_infos->GP(PROPBOL_VISIBLE) && Info::Get().Options().GetOptInfos())
		m_lb_infos->SP(PROPBOL_VISIBLE, true);
	//Change la texture de la barre de vie en fonction du %. Ne réassigne la texture que si on en a besoin
	if (val.Character->HealthPerc() <= PGB_HEALTH_LOW_TRESHOLD && m_pgb_health->GP(PROPTEXT_BAR) == val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH])
		m_pgb_health->SP(PROPTEXT_BAR, val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH_LOW]);
	else if (val.Character->HealthPerc() > PGB_HEALTH_LOW_TRESHOLD && m_pgb_health->GP(PROPTEXT_BAR) == val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH_LOW])
		m_pgb_health->SP(PROPTEXT_BAR, val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH]);
	//Affiche ou cache la barre d'énergie selon la situation
	if (val.Character->Energy() != val.Character->EnergyMax() || Info::Get().Ctrls().Shift())
	{
		m_pgb_energy->SP(PROPBOL_VISIBLE, true);
		m_lbl_energy->SP(PROPBOL_VISIBLE, true);
	}
	else if (val.Character->Energy() == val.Character->EnergyMax())
	{
		m_pgb_energy->SP(PROPBOL_VISIBLE, false);
		m_lbl_energy->SP(PROPBOL_VISIBLE, false);
	}
	// Controles de debug
	if (Info::Get().Options().GetOptDebug() && !m_lbl_FPS->GP(PROPBOL_VISIBLE))
	{
		m_lbl_plrPos->SP(PROPBOL_VISIBLE, true);
		m_lbl_plrSpd->SP(PROPBOL_VISIBLE, true);
		m_lbl_plrAcc->SP(PROPBOL_VISIBLE, true);
		m_lbl_mousePos->SP(PROPBOL_VISIBLE, true);
		m_lbl_FPS->SP(PROPBOL_VISIBLE, true);
	}
	else if (!Info::Get().Options().GetOptDebug() && m_lbl_FPS->GP(PROPBOL_VISIBLE))
	{
		m_lbl_plrPos->SP(PROPBOL_VISIBLE, false);
		m_lbl_plrSpd->SP(PROPBOL_VISIBLE, false);
		m_lbl_plrAcc->SP(PROPBOL_VISIBLE, false);
		m_lbl_mousePos->SP(PROPBOL_VISIBLE, false);
		m_lbl_FPS->SP(PROPBOL_VISIBLE, false);
	}

#pragma endregion

#pragma region Actualisation des valeurs

	//Actualisation des valeurs des progressbars
	m_pgb_health->SetValue(val.Character->HealthPerc());
	m_pgb_energy->SetValue(val.Character->EnergyPerc());
	m_pgb_mana->SetValue(val.Character->ManaPerc());
	m_pgb_exp->SetValue(val.Character->ExpPerc());

	//Actualisation du texte dans les différents Label
	TextUpdate(val);

#pragma endregion

#pragma region Ecriture console a partir de l exterieur

	if (Info::Get().LineToPrint() != "")
	{
		m_lb_console->AddLine(Info::Get().LineToPrint());
		Info::Get().NextPrint("");
	}

#pragma endregion
}

void GameInterface::TextUpdate(const ValuesGameInterface& val)
{
	std::ostringstream ss;
	//Vie
	ss << (int)val.Character->Health() << " / " << (int)val.Character->HealthMax();
	m_lbl_health->SP(PROPSTR_TEXT, ss.str());
	ss.str("");
	//Energie
	ss << (int)val.Character->Energy() << " / " << (int)val.Character->EnergyMax();
	m_lbl_energy->SP(PROPSTR_TEXT, ss.str());
	ss.str("");
	//Mana
	ss << (int)val.Character->Mana() << " / " << (int)val.Character->ManaMax();
	m_lbl_mana->SP(PROPSTR_TEXT, ss.str());
	ss.str("");
	//Experience
	ss << (int)val.Character->Exp() << " / " << (int)val.Character->ExpNext();
	m_lbl_exp->SP(PROPSTR_TEXT, ss.str());
	ss.str("");
	//Niveau
	ss << val.Character->Level();
	m_lbl_playerLevel->SP(PROPSTR_TEXT, ss.str());
	ss.str("");
	//Position
	ss << "Position :     ( " << std::setprecision(4) << val.Player->Position().x << ", " << std::setprecision(4) <<
		val.Player->Position().y << ", " << std::setprecision(4) << val.Player->Position().z << " )";
	m_lbl_plrPos->SP(PROPSTR_TEXT, ss.str());
	ss.str("");
	//Vitesse
	ss << "Vitesse :      " << val.Player->Speed();
	m_lbl_plrSpd->SP(PROPSTR_TEXT, ss.str());
	ss.str("");
	//Accélération
	ss << "Acceleration : " << val.Player->Acceleration();
	m_lbl_plrAcc->SP(PROPSTR_TEXT, ss.str());
	ss.str("");
	//Psotion souris
	ss << "Pos. Souris :  " << val.Mousepos;
	m_lbl_mousePos->SP(PROPSTR_TEXT, ss.str());
	ss.str("");
	//FPS
	ss << "Fps :          " << std::setprecision(2) << val.Fps;
	m_lbl_FPS->SP(PROPSTR_TEXT, ss.str());
	ss.str("");
	//Heure
	time_t currentTime;
	time (&currentTime);
	struct tm ptm;
	localtime_s(&ptm, &currentTime);
	ss << std::setfill('0');
	ss << std::setw(2) << ptm.tm_hour << ":" << std::setw(2) << ptm.tm_min;
	m_lbl_time->SP(PROPSTR_TEXT, ss.str());
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

void GameInterface::Init(const ValuesGameInterface& val)
{
	// Écran
	m_pnl_screen = new Panel();
	m_pnl_screen->CtrlInit(0, Vector2i(), Vector2i(val.Width, val.Height), 0, "main");
	m_pnl_screen->ContainInit(1);
	m_pnl_screen->Init();

#pragma region Enfants de Main

	// Zone de jeu
	m_pnl_playscreen = new Panel();
	m_pnl_playscreen->CtrlInit(m_pnl_screen, 
		Vector2i(INTERFACE_SIDE_LEFT_WIDTH, INTERFACE_BOTTOM_HEIGHT),
		Vector2i(m_pnl_screen->GP(PROPVCT2_SIZE).x - INTERFACE_SIDE_LEFT_WIDTH - INTERFACE_SIDE_RIGHT_WIDTH, 
		m_pnl_screen->GP(PROPVCT2_SIZE).y - INTERFACE_BOTTOM_HEIGHT - INTERFACE_TOP_HEIGHT),
		0, "playscreen");
	m_pnl_playscreen->ContainInit(PNL_PLAYSCREEN_CONTROLS_NBR);
	m_pnl_playscreen->Init();
	m_pnl_screen->AddControl(m_pnl_playscreen);

#pragma region Enfants de Playscreen

	// Panel welcome
	//m_pnl_welcome = new Panel(m_pnl_playscreen, Vector2i(m_pnl_playscreen->GP(PROPVCT2_SIZE).x / 2 - 300, 
	//	m_pnl_playscreen->GP(PROPVCT2_SIZE).y - 300), Vector2i(600, 200), val.UITextures[CUSTIMAGE_MENU_BACKGROUND],
	//	2, "welcomepanel");
	//m_pnl_playscreen->AddControl(m_pnl_welcome);
	//m_pnl_welcome->SP(PROPBOL_REPEATTEXTURE, false);

	//m_pb_welcomeface = new PictureBox(m_pnl_welcome, Vector2i(), Vector2i(200, 200), val.UITextures[CUSTIMAGE_WELCOME_FACE], "pb_welcome");
	//m_pnl_welcome->AddControl(m_pb_welcomeface);
	//m_pb_welcomeface->SP(PROPBOL_REPEATTEXTURE, false);

	//m_lbl_welcomemessage = new Label(m_pnl_welcome, Vector2i(), val.FontTextures[TEXTCOLOR_WHITE], "Welcome to the world of Cthulhu", 
	//	TEXTDOCK_MIDDLERIGHT, false, 32, 25, 0.5f, Vector2f(), "lblwelcome");
	//m_pnl_welcome->AddControl(m_lbl_welcomemessage);

	// Informations
	m_lb_infos = new ListBox();
	m_lb_infos->CtrlInit(m_pnl_playscreen, Vector2i(5, 
		m_pnl_playscreen->GP(PROPVCT2_SIZE).y - LBL_GENERIC_CHAR_H*6 - 26*12),
		Vector2i(), 0, "lb_infos");
	m_lb_infos->TextInit("", val.FontTextures[TEXTCOLOR_RED], false, 12.f, 12.f, 0.5f);
	m_lb_infos->Init(26, 2, Vector2i(), false);
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
	m_lb_console = new ListBox();
	m_lb_console->CtrlInit(m_pnl_playscreen, 
		Vector2i(m_pnl_playscreen->GP(PROPVCT2_SIZE).x - 64 - (int)LB_CONSOLE_SIZE_W, TXB_CONSOLE_SIZE_H + 5), 
		Vector2i(LB_CONSOLE_SIZE_W, TXB_CONSOLE_SIZE_H), val.UITextures[CUSTIMAGE_CONSOLE_BACK], LB_CONSOLE_NAME);
	m_lb_console->TextInit("", val.FontTextures[TEXTCOLOR_YELLOW], false, LB_CONSOLE_CHAR_H, LB_CONSOLE_CHAR_W, LB_CONSOLE_CHAR_I);
	m_lb_console->Init(LB_CONSOLE_LINE_NUMBER, LB_CONSOLE_LINE_GAP, Vector2i(LB_CONSOLE_BODER_OFFSET_S, LB_CONSOLE_BODER_OFFSET_B),
		true, val.UITextures[CUSTIMAGE_ARROWBUTTON_UP], val.UITextures[CUSTIMAGE_ARROWBUTTON_DOWN]);
	m_pnl_playscreen->AddControl(m_lb_console);
	Info::Get().SetConsole(m_lb_console);
	//Texbox de la console
	m_txb_console = new Textbox();
	m_txb_console->CtrlInit(m_pnl_playscreen, 
		Vector2i(m_pnl_playscreen->GP(PROPVCT2_SIZE).x - 64 - (int)LB_CONSOLE_SIZE_W, 0),
		Vector2i(TXB_CONSOLE_SIZE_W, TXB_CONSOLE_SIZE_H), val.UITextures[CUSTIMAGE_CONSOLE_TEXTBOX_BACK], TXB_CONSOLE_NAME);
	m_txb_console->TextInit("", val.FontTextures[TEXTCOLOR_WHITE], LBL_GENERIC_ITALIC, TXB_CONSOLE_SIZE_H * 0.75f, 
		TXB_CONSOLE_SIZE_H * 0.75f, TXB_CONSOLE_CHAR_I);
	m_txb_console->Init(Vector2i(TXB_CONSOLE_OFFSET_X, TXB_CONSOLE_OFFSET_Y));
	m_pnl_playscreen->AddControl(m_txb_console);
	m_txb_console->SP(PROPBOL_VISIBLE, false);
	// Frame portrait
	m_pnl_portrait = new Panel();
	m_pnl_portrait->CtrlInit(m_pnl_playscreen,
		Vector2i(PNL_PORTRAIT_POSITION_X, PNL_PORTRAIT_POSITION_Y),
		Vector2i(PNL_PORTRAIT_SIZE_W, PNL_PORTRAIT_SIZE_H),
		val.UITextures[CUSTIMAGE_PORTRAIT_FRAME], PNL_PORTRAIT_NAME);
	m_pnl_portrait->ContainInit(PNL_PORTRAIT_CONTROLS_NBR);
	m_pnl_portrait->Init();
	m_pnl_playscreen->AddControl(m_pnl_portrait);

#pragma region Enfants pnl portrait

	// Barre de vie
	m_pgb_health = new ProgressBar();
	m_pgb_health->CtrlInit(m_pnl_portrait, Vector2i(PGB_HEALTH_POSITION_X, PGB_HEALTH_POSITION_Y),
		Vector2i(PGB_HEALTH_SIZE_W, PGB_HEALTH_SIZE_H), val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH_BACK], PGB_HEALTH_NAME);
	m_pgb_health->Init(ProgressBar::BARMODE_HORIZONTAL_LTR, val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH],
		PGB_HEALTH_BACKGROUND, PGB_HEALTH_BORDER_SIZE);
	m_pnl_portrait->AddControl(m_pgb_health);
	// Barre de mana
	m_pgb_mana = new ProgressBar();
	m_pgb_mana->CtrlInit(m_pnl_portrait, Vector2i(PGB_MANA_POSITION_X, PGB_MANA_POSITION_Y),
		Vector2i(PGB_MANA_SIZE_W, PGB_MANA_SIZE_H), val.UITextures[CUSTIMAGE_PGBTEXT_MANA_BACK], PGB_MANA_NAME);
	m_pgb_mana->Init(ProgressBar::BARMODE_HORIZONTAL_LTR, val.UITextures[CUSTIMAGE_PGBTEXT_MANA],
		PGB_MANA_BACKGROUND, PGB_MANA_BORDER_SIZE);
	m_pnl_portrait->AddControl(m_pgb_mana);
	// Barre d'expérience
	m_pgb_exp = new ProgressBar();
	m_pgb_exp->CtrlInit(m_pnl_portrait, Vector2i(PGB_EXP_POSITION_X, PGB_EXP_POSITION_Y),
		Vector2i(PGB_EXP_SIZE_W, PGB_EXP_SIZE_H), val.UITextures[CUSTIMAGE_PGBTEXT_EXP_BACK], PGB_EXP_NAME);
	m_pgb_exp->Init(ProgressBar::BARMODE_HORIZONTAL_LTR, val.UITextures[CUSTIMAGE_PGBTEXT_EXP],
		PGB_EXP_BACKGROUND, PGB_EXP_BORDER_SIZE);
	m_pnl_portrait->AddControl(m_pgb_exp);
	m_lbl_health = new Label();
	m_lbl_health->CtrlInit(m_pnl_portrait, Vector2i(LBL_HEALTH_POSITION_X, LBL_HEALTH_POSITION_Y), Vector2i(), 0, LBL_HEALTH_NAME);
	m_lbl_health->TextInit("", val.FontTextures[TEXTCOLOR_RED], PNL_PORTRAIT_ITALIC, PNL_PORTRAIT_CHAR_H, PNL_PORTRAIT_CHAR_W, PNL_PORTRAIT_CHAR_I);
	m_lbl_health->Init(TEXTDOCK_NONE, Vector2f());
	m_pnl_portrait->AddControl(m_lbl_health);
	// Label de mana
	m_lbl_mana = new Label();
	m_lbl_mana->CtrlInit(m_pnl_portrait, Vector2i(LBL_MANA_POSITION_X, LBL_MANA_POSITION_Y), Vector2i(), 0, LBL_MANA_NAME);
	m_lbl_mana->TextInit("", val.FontTextures[TEXTCOLOR_BLUE], PNL_PORTRAIT_ITALIC, PNL_PORTRAIT_CHAR_H, PNL_PORTRAIT_CHAR_W, PNL_PORTRAIT_CHAR_I);
	m_lbl_mana->Init(TEXTDOCK_NONE, Vector2f());
	m_pnl_portrait->AddControl(m_lbl_mana);
	// Label d'exp
	m_lbl_exp = new Label();
	m_lbl_exp->CtrlInit(m_pnl_portrait, Vector2i(LBL_EXP_POSITION_X, LBL_EXP_POSITION_Y), Vector2i(), 0, LBL_EXP_NAME);
	m_lbl_exp->TextInit("", val.FontTextures[TEXTCOLOR_YELLOW], PNL_PORTRAIT_ITALIC, PNL_PORTRAIT_CHAR_H, PNL_PORTRAIT_CHAR_W, PNL_PORTRAIT_CHAR_I);
	m_lbl_exp->Init(TEXTDOCK_NONE, Vector2f());
	m_pnl_portrait->AddControl(m_lbl_exp);

	// Image du joueur
	m_pnl_playerImage = new Panel();
	m_pnl_playerImage->CtrlInit(m_pnl_portrait, 
		Vector2i(PB_PORTRAIT_POSITION_X, PB_PORTRAIT_POSITION_Y),
		Vector2i(PB_PORTRAIT_SIZE_W, PB_PORTRAIT_SIZE_H),
		val.UITextures[CUSTIMAGE_PORTRAIT_MALE], PB_PORTRAIT_NAME);
	m_pnl_playerImage->ContainInit(1);
	m_pnl_playerImage->Init();
	m_pnl_portrait->AddControl(m_pnl_playerImage);

#pragma region Enfants de m_pnl_playerImage

	m_lbl_playerLevel = new Label();
	m_lbl_playerLevel->CtrlInit(m_pnl_playerImage, Vector2i(LBL_EXP_POSITION_X, LBL_EXP_POSITION_Y), Vector2i(), 0, LBL_PLAYER_LEVEL_NAME);
	m_lbl_playerLevel->TextInit("", val.FontTextures[TEXTCOLOR_BLUE], PNL_PORTRAIT_ITALIC, LBL_PLAYER_LEVEL_H, LBL_PLAYER_LEVEL_W, LBL_PLAYER_LEVEL_I);
	m_lbl_playerLevel->Init(TEXTDOCK_TOPCENTER, Vector2f(0, -10));
	m_pnl_playerImage->AddControl(m_lbl_playerLevel);

#pragma endregion

	m_lbl_currentBlockType = new Label();
	m_lbl_currentBlockType->CtrlInit(m_pnl_playscreen, m_pnl_playerImage->GP(PROPVCT2_POSITION) + Vector2i(0, 200), 
		Vector2i(), 0, "lbl_blocinfo");
	m_lbl_currentBlockType->TextInit("Bloc : ", val.FontTextures[TEXTCOLOR_BLUE], false, 
		LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_lbl_currentBlockType->Init(TEXTDOCK_NONE, Vector2f());
	m_pb_currentBlockType = new PictureBox();
	m_pb_currentBlockType->CtrlInit(m_pnl_playscreen, 
		Vector2i(m_lbl_currentBlockType->GP(PROPVCT2_POSITION).x + 84, 
		m_lbl_currentBlockType->GP(PROPVCT2_POSITION).y),
		Vector2i(40,40), 0, "pbcurbloc");
	m_pb_currentBlockType->Init();
	m_pnl_playscreen->AddControl(m_lbl_currentBlockType);
	m_pnl_playscreen->AddControl(m_pb_currentBlockType);

#pragma endregion

	//Barre d'énergie verticale
	m_pgb_energy = new ProgressBar();
	m_pgb_energy->CtrlInit(m_pnl_playscreen, Vector2i(PGB_ENERGY_POSITION_X, PGB_ENERGY_POSITION_Y),
		Vector2i(PGB_ENERGY_SIZE_W, PGB_ENERGY_SIZE_H), val.UITextures[CUSTIMAGE_PGBTEXT_ENERGY_BACK], PGB_ENERGY_NAME);
	m_pgb_energy->Init(ProgressBar::BARMODE_VERTICAL_DTU, val.UITextures[CUSTIMAGE_PGBTEXT_ENERGY],
		PGB_ENERGY_BACKGROUND, PGB_ENERGY_BORDER_SIZE);
	m_pnl_playscreen->AddControl(m_pgb_energy);
	//Label d'énergie
	m_lbl_energy = new Label();
	m_lbl_energy->CtrlInit(m_pnl_playscreen, Vector2i(LBL_ENERGY_POSITION_X, LBL_ENERGY_POSITION_Y), Vector2i(), 0, LBL_ENERGY_NAME);
	m_lbl_energy->TextInit("", val.FontTextures[TEXTCOLOR_GREEN], LBL_ENERGY_ITALIC, LBL_ENERGY_CHAR_H, LBL_ENERGY_CHAR_W, LBL_ENERGY_CHAR_I);
	m_lbl_energy->Init(TEXTDOCK_NONE, Vector2f());
	m_pnl_playscreen->AddControl(m_lbl_energy);

#pragma region Controles de debug

	//Label Position
	m_lbl_plrPos = new Label();
	m_lbl_plrPos->CtrlInit(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->GP(PROPVCT2_SIZE).y - LBL_GENERIC_CHAR_H),
		Vector2i(), 0, "pos");
	m_lbl_plrPos->TextInit("", val.FontTextures[TEXTCOLOR_GREEN], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_lbl_plrPos->Init(TEXTDOCK_NONE, Vector2f());
	m_pnl_playscreen->AddControl(m_lbl_plrPos);
	//Label Vitesse
	m_lbl_plrSpd = new Label();
	m_lbl_plrSpd->CtrlInit(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->GP(PROPVCT2_SIZE).y - LBL_GENERIC_CHAR_H * 2),
		Vector2i(), 0, "spd");
	m_lbl_plrSpd->TextInit("", val.FontTextures[TEXTCOLOR_BLUE], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_lbl_plrSpd->Init(TEXTDOCK_NONE, Vector2f());
	m_pnl_playscreen->AddControl(m_lbl_plrSpd);
	//Label Acceleration
	m_lbl_plrAcc = new Label();
	m_lbl_plrAcc->CtrlInit(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->GP(PROPVCT2_SIZE).y - LBL_GENERIC_CHAR_H * 3),
		Vector2i(), 0, "acc");
	m_lbl_plrAcc->TextInit("", val.FontTextures[TEXTCOLOR_RED], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_lbl_plrAcc->Init(TEXTDOCK_NONE, Vector2f());
	m_pnl_playscreen->AddControl(m_lbl_plrAcc);
	//Label mouse position
	m_lbl_mousePos = new Label();
	m_lbl_mousePos->CtrlInit(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->GP(PROPVCT2_SIZE).y - LBL_GENERIC_CHAR_H * 4),
		Vector2i(), 0, "mpos");
	m_lbl_mousePos->TextInit("", val.FontTextures[TEXTCOLOR_WHITE], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_lbl_mousePos->Init(TEXTDOCK_NONE, Vector2f());
	m_pnl_playscreen->AddControl(m_lbl_mousePos);
	//Label FPS
	m_lbl_FPS = new Label();
	m_lbl_FPS->CtrlInit(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->GP(PROPVCT2_SIZE).y - LBL_GENERIC_CHAR_H * 5),
		Vector2i(), 0, "fps");
	m_lbl_FPS->TextInit("", val.FontTextures[TEXTCOLOR_YELLOW], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_lbl_FPS->Init(TEXTDOCK_NONE, Vector2f());
	m_pnl_playscreen->AddControl(m_lbl_FPS);

#pragma endregion

	//Heure
	m_pnl_time = new Panel();
	m_pnl_time->CtrlInit(m_pnl_playscreen, Vector2i(m_pnl_playscreen->GP(PROPVCT2_SIZE).x - 128, m_pnl_playscreen->GP(PROPVCT2_SIZE).y - 64), 
		Vector2i(128, 64), val.UITextures[CUSTIMAGE_CLOCK_BG], "clock");
	m_pnl_time->ContainInit(1);
	m_pnl_time->Init();
	m_pnl_playscreen->AddControl(m_pnl_time);

#pragma region Enfants de m_pnl_time

	m_lbl_time = new Label();
	m_lbl_time->CtrlInit(m_pnl_time, Vector2i(), Vector2i(), 0, "time");
	m_lbl_time->TextInit("", val.FontTextures[TEXTCOLOR_WHITE], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_lbl_time->Init(TEXTDOCK_MIDDLECENTER, Vector2f());
	m_pnl_time->AddControl(m_lbl_time);

#pragma endregion

#pragma endregion

#pragma endregion

#pragma endregion

	m_loaded = true;
}

void GameInterface::Render()
{
	m_pnl_playscreen->Render();
}

bool GameInterface::MousePressEvent( const MOUSE_BUTTON &button, int x, int y )
{
	Vector2i& pos = m_lb_console->AbsolutePosition();
	Vector2i& size = m_lb_console->GP(PROPVCT2_SIZE);
	Vector2i& play = m_pnl_screen->GP(PROPVCT2_SIZE);
	switch (button)
	{
	case MOUSE_BUTTON_RIGHT:
		break;

	case MOUSE_BUTTON_LEFT:
		if (m_lb_console->MousePressEvents(x, m_pnl_playscreen->GP(PROPVCT2_SIZE).y - y))
			return true;
		break;

	case MOUSE_BUTTON_WHEEL_UP:
		if (x >= pos.x && x <= pos.x + size.x && play.y - y <= pos.y + size.y && play.y - y >= pos.y) {
			m_lb_console->Scroll(1);
			return true;
		} 
		break;

	case MOUSE_BUTTON_WHEEL_DOWN:
		if (x >= pos.x && x <= pos.x + size.x && play.y - y <= pos.y + size.y && play.y - y >= pos.y) {
			m_lb_console->Scroll(-1);
			return true;
		} 
		break;
	}
	return false;
}

bool GameInterface::MouseRleaseEvent( const MOUSE_BUTTON &button, int x, int y )
{
	switch (button)
	{
	case MOUSE_BUTTON_RIGHT:
		break;

	case MOUSE_BUTTON_LEFT:
		m_lb_console->MouseReleaseEvents(x,y);
		break;
	}
	return false;
}

void GameInterface::ConsoleWriteLine( const std::string& line )
{
	m_lb_console->AddLine(line);
}

bool GameInterface::KeyReleaseEvent( unsigned char key )
{
	if (m_txb_console->HasFocus())
		return true;

	return false;
}

bool GameInterface::TextenteredEvent( unsigned int val )
{
	std::ostringstream ss;

	//Appuie sur Enter pour acquérir le focus
	if (val == 13 && !m_txb_console->HasFocus())
	{
		m_txb_console->SetFocus(true);
		m_txb_console->SP(PROPBOL_VISIBLE, true);
		return true;
	}

	//Capture du texte en mode Textbox
	if (m_txb_console->HasFocus())
	{
		//Vérification qu'il s'agit d'un caractère valide
		if (val >= 32 && val <= 126)
		{
			ss << m_txb_console->GetMsg() << static_cast<char>(val);
			m_txb_console->SetMessage(ss.str());
			ss.str("");
			return true;
		}
		//Si c'est un backspace
		if (val == 8)
		{
			std::string mes = m_txb_console->GetMsg();
			if (mes.length() > 0)
			{
				for (int i = 0; i < mes.length()-1; i++)
				{
					ss << mes[i];
				}
				m_txb_console->SetMessage(ss.str());
				ss.str("");
			}
		}
		//Si c'est un return
		if (val == 13)
		{
			if (m_txb_console->GetMsg() != "")
				ConsoleWriteLine(m_txb_console->GetMsg());
			m_txb_console->SP(PROPBOL_VISIBLE, false);
			m_txb_console->SetFocus(false);
			if (m_txb_console->GetMsg() == "/dance")
				Info::Get().Sound().PlaySnd(Son::SON_DEATH, Son::CHANNEL_INTERFACE);
			m_txb_console->SetMessage("");
		}
	}
	return false;
}

bool GameInterface::KeyPressEvent( unsigned char key )
{
	if (m_txb_console->HasFocus())
		return true;

	return false;
}
