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
	m_pb_currentBlockType->SetTexture(val.BlocTextures->GetTexture(
		Info::Get().GetBlocInfo(val.CurrentBlocType)->GetTextureIndex(0)));
	//Affiche ou cache les infos s'il y a un changement
	if (m_lb_infos->GetProperty(Control::PROPBOL_VISIBLE) && !Info::Get().Options().GetOptInfos())
		m_lb_infos->SetProperty(Control::PROPBOL_VISIBLE, false);
	else if (!m_lb_infos->GetProperty(Control::PROPBOL_VISIBLE) && Info::Get().Options().GetOptInfos())
		m_lb_infos->SetProperty(Control::PROPBOL_VISIBLE, true);
	//Change la texture de la barre de vie en fonction du %. Ne r�assigne la texture que si on en a besoin
	if (val.Character->HealthPerc() <= PGB_HEALTH_LOW_TRESHOLD && m_pgb_health->GetProperty(Control::PROPTEXT_BAR) == val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH])
		m_pgb_health->SetProperty(Control::PROPTEXT_BAR, val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH_LOW]);
	else if (val.Character->HealthPerc() > PGB_HEALTH_LOW_TRESHOLD && m_pgb_health->GetProperty(Control::PROPTEXT_BAR) == val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH_LOW])
		m_pgb_health->SetProperty(Control::PROPTEXT_BAR, val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH]);
	//Affiche ou cache la barre d'�nergie selon la situation
	if (val.Character->Energy() != val.Character->EnergyMax() || Info::Get().Ctrls().Shift())
	{
		m_pgb_energy->SetProperty(Control::PROPBOL_VISIBLE, true);
		m_lbl_energy->SetProperty(Control::PROPBOL_VISIBLE, true);
	}
	else if (val.Character->Energy() == val.Character->EnergyMax())
	{
		m_pgb_energy->SetProperty(Control::PROPBOL_VISIBLE, false);
		m_lbl_energy->SetProperty(Control::PROPBOL_VISIBLE, false);
	}
	// Controles de debug
	if (Info::Get().Options().GetOptDebug() && !m_lbl_FPS->GetProperty(Control::PROPBOL_VISIBLE))
	{
		m_lbl_plrPos->SetProperty(Control::PROPBOL_VISIBLE, true);
		m_lbl_plrSpd->SetProperty(Control::PROPBOL_VISIBLE, true);
		m_lbl_plrAcc->SetProperty(Control::PROPBOL_VISIBLE, true);
		m_lbl_mousePos->SetProperty(Control::PROPBOL_VISIBLE, true);
		m_lbl_FPS->SetProperty(Control::PROPBOL_VISIBLE, true);
	}
	else if (!Info::Get().Options().GetOptDebug() && m_lbl_FPS->GetProperty(Control::PROPBOL_VISIBLE))
	{
		m_lbl_plrPos->SetProperty(Control::PROPBOL_VISIBLE, false);
		m_lbl_plrSpd->SetProperty(Control::PROPBOL_VISIBLE, false);
		m_lbl_plrAcc->SetProperty(Control::PROPBOL_VISIBLE, false);
		m_lbl_mousePos->SetProperty(Control::PROPBOL_VISIBLE, false);
		m_lbl_FPS->SetProperty(Control::PROPBOL_VISIBLE, false);
	}

#pragma endregion

#pragma region Actualisation des valeurs

	//Actualisation des valeurs des progressbars
	m_pgb_health->SetValue(val.Character->HealthPerc());
	m_pgb_energy->SetValue(val.Character->EnergyPerc());
	m_pgb_mana->SetValue(val.Character->ManaPerc());
	m_pgb_exp->SetValue(val.Character->ExpPerc());

	//Actualisation du texte dans les diff�rents Label
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
	m_lbl_health->SetProperty(Label::PropString::PROPSTR_TEXT, ss.str());
	ss.str("");
	//Energie
	ss << (int)val.Character->Energy() << " / " << (int)val.Character->EnergyMax();
	m_lbl_energy->SetProperty(Label::PropString::PROPSTR_TEXT, ss.str());
	ss.str("");
	//Mana
	ss << (int)val.Character->Mana() << " / " << (int)val.Character->ManaMax();
	m_lbl_mana->SetProperty(Label::PropString::PROPSTR_TEXT, ss.str());
	ss.str("");
	//Experience
	ss << (int)val.Character->Exp() << " / " << (int)val.Character->ExpNext();
	m_lbl_exp->SetProperty(Label::PropString::PROPSTR_TEXT, ss.str());
	ss.str("");
	//Niveau
	ss << val.Character->Level();
	m_lbl_playerLevel->SetProperty(Label::PropString::PROPSTR_TEXT, ss.str());
	ss.str("");
	//Position
	ss << "Position :     ( " << std::setprecision(4) << val.Player->Position().x << ", " << std::setprecision(4) <<
		val.Player->Position().y << ", " << std::setprecision(4) << val.Player->Position().z << " )";
	m_lbl_plrPos->SetProperty(Label::PropString::PROPSTR_TEXT, ss.str());
	ss.str("");
	//Vitesse
	ss << "Vitesse :      " << val.Player->Speed();
	m_lbl_plrSpd->SetProperty(Label::PropString::PROPSTR_TEXT, ss.str());
	ss.str("");
	//Acc�l�ration
	ss << "Acceleration : " << val.Player->Acceleration();
	m_lbl_plrAcc->SetProperty(Label::PropString::PROPSTR_TEXT, ss.str());
	ss.str("");
	//Psotion souris
	ss << "Pos. Souris :  " << val.Mousepos;
	m_lbl_mousePos->SetProperty(Label::PropString::PROPSTR_TEXT, ss.str());
	ss.str("");
	//FPS
	ss << "Fps :          " << std::setprecision(2) << val.Fps;
	m_lbl_FPS->SetProperty(Label::PropString::PROPSTR_TEXT, ss.str());
	ss.str("");
	//Heure
	time_t currentTime;
	time (&currentTime);
	struct tm ptm;
	localtime_s(&ptm, &currentTime);
	ss << std::setfill('0');
	ss << std::setw(2) << ptm.tm_hour << ":" << std::setw(2) << ptm.tm_min;
	m_lbl_time->SetProperty(Label::PropString::PROPSTR_TEXT, ss.str());
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
	// �cran
	m_pnl_screen = new Panel();
	m_pnl_screen->PreInit(0, Vector2i(), Vector2i(val.Width, val.Height), 0, "main");
	m_pnl_screen->Init(1);

#pragma region Enfants de Main

	// Zone de jeu
	m_pnl_playscreen = new Panel();
	m_pnl_playscreen->PreInit(m_pnl_screen, 
		Vector2i(INTERFACE_SIDE_LEFT_WIDTH, INTERFACE_BOTTOM_HEIGHT),
		Vector2i(m_pnl_screen->GetProperty(Control::PROPVCT2_SIZE).x - INTERFACE_SIDE_LEFT_WIDTH - INTERFACE_SIDE_RIGHT_WIDTH, 
		m_pnl_screen->GetProperty(Control::PROPVCT2_SIZE).y - INTERFACE_BOTTOM_HEIGHT - INTERFACE_TOP_HEIGHT),
		0, "playscreen");
	m_pnl_playscreen->Init(PNL_PLAYSCREEN_CONTROLS_NBR);
	m_pnl_screen->AddControl(m_pnl_playscreen);

#pragma region Enfants de Playscreen

	// Panel welcome
	//m_pnl_welcome = new Panel(m_pnl_playscreen, Vector2i(m_pnl_playscreen->GetProperty(Control::PROPVCT2_SIZE).x / 2 - 300, 
	//	m_pnl_playscreen->GetProperty(Control::PROPVCT2_SIZE).y - 300), Vector2i(600, 200), val.UITextures[CUSTIMAGE_MENU_BACKGROUND],
	//	2, "welcomepanel");
	//m_pnl_playscreen->AddControl(m_pnl_welcome);
	//m_pnl_welcome->SetProperty(Control::PROPBOL_REPEATTEXTURE, false);

	//m_pb_welcomeface = new PictureBox(m_pnl_welcome, Vector2i(), Vector2i(200, 200), val.UITextures[CUSTIMAGE_WELCOME_FACE], "pb_welcome");
	//m_pnl_welcome->AddControl(m_pb_welcomeface);
	//m_pb_welcomeface->SetProperty(Control::PROPBOL_REPEATTEXTURE, false);

	//m_lbl_welcomemessage = new Label(m_pnl_welcome, Vector2i(), val.FontTextures[TEXTCOLOR_WHITE], "Welcome to the world of Cthulhu", 
	//	Label::TEXTDOCK_MIDDLERIGHT, false, 32, 25, 0.5f, Vector2f(), "lblwelcome");
	//m_pnl_welcome->AddControl(m_lbl_welcomemessage);

	// Informations
	m_lb_infos = new ListBox(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->GetProperty(Control::PROPVCT2_SIZE).y - LBL_GENERIC_CHAR_H*6 - 26*12), 200, val.FontTextures[TEXTCOLOR_RED], 
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
		Vector2i(m_pnl_playscreen->GetProperty(Control::PROPVCT2_SIZE).x - 64 - (int)LB_CONSOLE_SIZE_W, TXB_CONSOLE_SIZE_H + 5), 
		LB_CONSOLE_SIZE_W, 
		val.FontTextures[TEXTCOLOR_YELLOW], 
		val.UITextures[CUSTIMAGE_CONSOLE_BACK], 
		LB_CONSOLE_LINE_NUMBER, 
		LB_CONSOLE_LINE_GAP, 
		LB_CONSOLE_CHAR_W, 
		LB_CONSOLE_CHAR_H, 
		LB_CONSOLE_CHAR_I, 
		LB_CONSOLE_SCROLLABLE, 
		LB_CONSOLE_NAME,
		Vector2i(LB_CONSOLE_BODER_OFFSET_S, LB_CONSOLE_BODER_OFFSET_B));
	m_pnl_playscreen->AddControl(m_lb_console);
	m_lb_console->SetProperty(Control::PROPBOL_REPEATTEXTURE, false);
	Info::Get().SetConsole(m_lb_console);
	//Texbox de la console
	m_txb_console = new Textbox(m_pnl_playscreen,
		Vector2i(m_pnl_playscreen->GetProperty(Control::PROPVCT2_SIZE).x - 64 - (int)LB_CONSOLE_SIZE_W, 0),
		Vector2i(TXB_CONSOLE_SIZE_W, TXB_CONSOLE_SIZE_H),
		val.FontTextures[TEXTCOLOR_WHITE],
		val.UITextures[CUSTIMAGE_CONSOLE_TEXTBOX_BACK],
		Label::TEXTDOCK_MIDDLELEFT,
		LBL_GENERIC_ITALIC,
		TXB_CONSOLE_SIZE_H * 0.75f,
		TXB_CONSOLE_CHAR_I,
		Vector2f(TXB_CONSOLE_OFFSET_X, TXB_CONSOLE_OFFSET_Y),
		TXB_CONSOLE_NAME);
	m_pnl_playscreen->AddControl(m_txb_console);
	m_txb_console->SetProperty(Control::PROPBOL_REPEATTEXTURE, false);
	m_txb_console->SetProperty(Control::PROPBOL_VISIBLE, false);
	m_txb_console->SetFocus(false);
	// Frame portrait
	m_pnl_portrait = new Panel();
	m_pnl_portrait->PreInit(m_pnl_playscreen,
		Vector2i(PNL_PORTRAIT_POSITION_X, PNL_PORTRAIT_POSITION_Y),
		Vector2i(PNL_PORTRAIT_SIZE_W, PNL_PORTRAIT_SIZE_H),
		val.UITextures[CUSTIMAGE_PORTRAIT_FRAME], PNL_PORTRAIT_NAME);
	m_pnl_portrait->Init(PNL_PORTRAIT_CONTROLS_NBR);
	m_pnl_playscreen->AddControl(m_pnl_portrait);

	m_lbl_currentBlockType = new Label(m_pnl_playscreen, Vector2i(m_pnl_portrait->GetProperty(Panel::PROPVCT2_POSITION).x,
		m_pnl_portrait->GetProperty(Panel::PROPVCT2_POSITION).y + m_pnl_portrait->GetProperty(Panel::PROPVCT2_SIZE).y + 10),
		val.FontTextures[TEXTCOLOR_BLUE], "Bloc : ", Label::TEXTDOCK_NONE, false, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "lblcurblock");
	m_pb_currentBlockType = new PictureBox(m_pnl_playscreen, Vector2i(m_lbl_currentBlockType->GetProperty(Panel::PROPVCT2_POSITION).x + 84, m_lbl_currentBlockType->GetProperty(Panel::PROPVCT2_POSITION).y), Vector2i(20,20), 
		val.BlocTextures->GetTexture(val.CurrentBlocType - 1), "pbcurbloc");
	m_pnl_playscreen->AddControl(m_lbl_currentBlockType);
	m_pnl_playscreen->AddControl(m_pb_currentBlockType);

#pragma region Enfants pnl portrait

	// Barre de vie
	m_pgb_health = new ProgressBar(m_pnl_portrait,
		Vector2i(PGB_HEALTH_POSITION_X, PGB_HEALTH_POSITION_Y),
		Vector2i(PGB_HEALTH_SIZE_W, PGB_HEALTH_SIZE_H),
		val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH], val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH_BACK],
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_HEALTH_BACKGROUND, PGB_HEALTH_BORDER_SIZE, PGB_HEALTH_NAME);
	m_pnl_portrait->AddControl(m_pgb_health);
	// Barre de mana
	m_pgb_mana = new ProgressBar(m_pnl_portrait,
		Vector2i(PGB_MANA_POSITION_X, PGB_MANA_POSITION_Y),
		Vector2i(PGB_MANA_SIZE_W, PGB_MANA_SIZE_H),
		val.UITextures[CUSTIMAGE_PGBTEXT_MANA], val.UITextures[CUSTIMAGE_PGBTEXT_MANA_BACK],
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_MANA_BACKGROUND, PGB_MANA_BORDER_SIZE, PGB_MANA_NAME);
	m_pnl_portrait->AddControl(m_pgb_mana);
	// Barre d'exp�rience
	m_pgb_exp = new ProgressBar(m_pnl_portrait,
		Vector2i(PGB_EXP_POSITION_X, PGB_EXP_POSITION_Y),
		Vector2i(PGB_EXP_SIZE_W, PGB_EXP_SIZE_H),
		val.UITextures[CUSTIMAGE_PGBTEXT_EXP], val.UITextures[CUSTIMAGE_PGBTEXT_EXP_BACK],
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_EXP_BACKGROUND, PGB_EXP_BORDER_SIZE, PGB_EXP_NAME);
	m_pnl_portrait->AddControl(m_pgb_exp);
	m_lbl_health = new Label(m_pnl_portrait, Vector2i(LBL_HEALTH_POSITION_X, LBL_HEALTH_POSITION_Y), val.FontTextures[TEXTCOLOR_RED], "", 
		Label::TEXTDOCK_NONE, PNL_PORTRAIT_ITALIC, PNL_PORTRAIT_CHAR_H, PNL_PORTRAIT_CHAR_W, PNL_PORTRAIT_CHAR_I, Vector2f(), LBL_HEALTH_NAME);
	m_pnl_portrait->AddControl(m_lbl_health);
	// Label de mana
	m_lbl_mana = new Label(m_pnl_portrait, Vector2i(LBL_MANA_POSITION_X, LBL_MANA_POSITION_Y), val.FontTextures[TEXTCOLOR_BLUE], "", 
		Label::TEXTDOCK_NONE, PNL_PORTRAIT_ITALIC, PNL_PORTRAIT_CHAR_H, PNL_PORTRAIT_CHAR_W, PNL_PORTRAIT_CHAR_I, Vector2f(), LBL_MANA_NAME);
	m_pnl_portrait->AddControl(m_lbl_mana);
	// Label d'exp
	m_lbl_exp = new Label(m_pnl_portrait, Vector2i(LBL_EXP_POSITION_X, LBL_EXP_POSITION_Y), val.FontTextures[TEXTCOLOR_YELLOW], "", 
		Label::TEXTDOCK_NONE, PNL_PORTRAIT_ITALIC, LBL_EXP_CHAR_W, LBL_EXP_CHAR_H, PNL_PORTRAIT_CHAR_I, Vector2f(), LBL_EXP_NAME);
	m_pnl_portrait->AddControl(m_lbl_exp);

	// Image du joueur
	m_pnl_playerImage = new Panel();
	m_pnl_playerImage->PreInit(m_pnl_portrait, 
		Vector2i(PB_PORTRAIT_POSITION_X, PB_PORTRAIT_POSITION_Y),
		Vector2i(PB_PORTRAIT_SIZE_W, PB_PORTRAIT_SIZE_H),
		val.UITextures[CUSTIMAGE_PORTRAIT_MALE], PB_PORTRAIT_NAME);
	m_pnl_playerImage->Init(1);
	m_pnl_portrait->AddControl(m_pnl_playerImage);

#pragma region Enfants de m_pnl_playerImage

	m_lbl_playerLevel = new Label(m_pnl_playerImage, Vector2i(), val.FontTextures[TEXTCOLOR_BLUE], "", 
		Label::TEXTDOCK_TOPCENTER, LBL_GENERIC_ITALIC, LBL_PLAYER_LEVEL_W, LBL_PLAYER_LEVEL_H, LBL_PLAYER_LEVEL_I, Vector2f(), LBL_PLAYER_LEVEL_NAME);
	m_pnl_playerImage->AddControl(m_lbl_playerLevel);

#pragma endregion

#pragma endregion

	//Barre d'�nergie verticale
	m_pgb_energy = new ProgressBar(m_pnl_playscreen,
		Vector2i(PGB_ENERGY_POSITION_X, PGB_ENERGY_POSITION_Y),
		Vector2i(PGB_ENERGY_SIZE_W, PGB_ENERGY_SIZE_H),
		val.UITextures[CUSTIMAGE_PGBTEXT_ENERGY], val.UITextures[CUSTIMAGE_PGBTEXT_ENERGY_BACK],
		ProgressBar::BARMODE_VERTICAL_DTU, PGB_ENERGY_BACKGROUND, PGB_ENERGY_BORDER_SIZE, PGB_ENERGY_NAME);
	m_pnl_playscreen->AddControl(m_pgb_energy);
	//Label d'�nergie
	m_lbl_energy = new Label(m_pnl_playscreen, Vector2i(LBL_ENERGY_POSITION_X, LBL_ENERGY_POSITION_Y), val.FontTextures[TEXTCOLOR_GREEN], "", 
		Label::TEXTDOCK_NONE, LBL_ENERGY_ITALIC, LBL_ENERGY_CHAR_H, LBL_ENERGY_CHAR_W, LBL_ENERGY_CHAR_I, Vector2f(), LBL_ENERGY_NAME);
	m_pnl_playscreen->AddControl(m_lbl_energy);

#pragma region Controles de debug

	//Label Position
	m_lbl_plrPos = new Label(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->GetProperty(Panel::PROPVCT2_SIZE).y - LBL_GENERIC_CHAR_H), val.FontTextures[TEXTCOLOR_GREEN], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "pos");
	m_pnl_playscreen->AddControl(m_lbl_plrPos);
	//Label Vitesse
	m_lbl_plrSpd = new Label(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->GetProperty(Panel::PROPVCT2_SIZE).y - LBL_GENERIC_CHAR_H*2), val.FontTextures[TEXTCOLOR_BLUE], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "spd");
	m_pnl_playscreen->AddControl(m_lbl_plrSpd);
	//Label Acceleration
	m_lbl_plrAcc = new Label(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->GetProperty(Panel::PROPVCT2_SIZE).y - LBL_GENERIC_CHAR_H*3), val.FontTextures[TEXTCOLOR_RED], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "acc");
	m_pnl_playscreen->AddControl(m_lbl_plrAcc);
	//Label mouse position
	m_lbl_mousePos = new Label(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->GetProperty(Panel::PROPVCT2_SIZE).y - LBL_GENERIC_CHAR_H*4), val.FontTextures[TEXTCOLOR_WHITE], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "Mpos");
	m_pnl_playscreen->AddControl(m_lbl_mousePos);
	//Label FPS
	m_lbl_FPS = new Label(m_pnl_playscreen, Vector2i(5, m_pnl_playscreen->GetProperty(Panel::PROPVCT2_SIZE).y - LBL_GENERIC_CHAR_H*5), val.FontTextures[TEXTCOLOR_YELLOW], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "fps");
	m_pnl_playscreen->AddControl(m_lbl_FPS);

#pragma endregion

	//Heure
	m_pnl_time = new Panel();
	m_pnl_time->PreInit(m_pnl_playscreen, Vector2i(m_pnl_playscreen->GetProperty(Panel::PROPVCT2_SIZE).x - 128, m_pnl_playscreen->GetProperty(Panel::PROPVCT2_SIZE).y - 64), 
		Vector2i(128, 64), val.UITextures[CUSTIMAGE_CLOCK_BG], "clock");
	m_pnl_time->Init(1);
	m_pnl_playscreen->AddControl(m_pnl_time);

#pragma region Enfants de m_pnl_time

	m_lbl_time = new Label(m_pnl_time, Vector2i(0,0), val.FontTextures[TEXTCOLOR_WHITE], "", Label::TEXTDOCK_MIDDLECENTER, false, 
		LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "time");
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
	Vector2i& size = m_lb_console->GetProperty(Control::PROPVCT2_SIZE);
	Vector2i& play = m_pnl_screen->GetProperty(Control::PROPVCT2_SIZE);
	switch (button)
	{
	case MOUSE_BUTTON_RIGHT:
		break;

	case MOUSE_BUTTON_LEFT:
		if (m_lb_console->MousePressEvents(x, m_pnl_playscreen->GetProperty(Control::PROPVCT2_SIZE).y - y))
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

	//Appuie sur Enter pour acqu�rir le focus
	if (val == 13 && !m_txb_console->HasFocus())
	{
		m_txb_console->SetFocus(true);
		m_txb_console->SetProperty(Control::PROPBOL_VISIBLE, true);
		return true;
	}

	//Capture du texte en mode Textbox
	if (m_txb_console->HasFocus())
	{
		//V�rification qu'il s'agit d'un caract�re valide
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
			m_txb_console->SetProperty(Control::PROPBOL_VISIBLE, false);
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
