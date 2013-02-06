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
	m_pb_currentBlockType->SetBackground(val.BlocTextures->GetTexture(
		Info::Get().GetBlocInfo(val.CurrentBlocType)->GetTextureIndex(0)));
	//Affiche ou cache les infos s'il y a un changement
	if (m_lb_infos->IsVisible() && !Info::Get().Options().GetOptInfos())
		m_lb_infos->Hide();
	else if (!m_lb_infos->IsVisible() && Info::Get().Options().GetOptInfos())
		m_lb_infos->Show();
	//Change la texture de la barre de vie en fonction du %. Ne réassigne la texture que si on en a besoin
	if (val.Character->HealthPerc() <= PGB_HEALTH_LOW_TRESHOLD && m_pgb_health->IsForeground(val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH]))
		m_pgb_health->SetForeground(val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH_LOW]);
	else if (val.Character->HealthPerc() > PGB_HEALTH_LOW_TRESHOLD && m_pgb_health->IsForeground(val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH_LOW]))
		m_pgb_health->SetForeground(val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH]);
	//Affiche ou cache la barre d'énergie selon la situation
	if (val.Character->Energy() != val.Character->EnergyMax() || Info::Get().Ctrls().Shift())
	{
		m_pgb_energy->Show();
		m_lbl_energy->Show();
	}
	else if (val.Character->Energy() == val.Character->EnergyMax())
	{
		m_pgb_energy->Hide();
		m_lbl_energy->Hide();
	}
	// Controles de debug
	if (Info::Get().Options().GetOptDebug() && !m_lbl_FPS->IsVisible())
	{
		m_lbl_plrPos->Show();
		m_lbl_plrSpd->Show();
		m_lbl_plrAcc->Show();
		m_lbl_mousePos->Show();
		m_lbl_FPS->Show();
	}
	else if (!Info::Get().Options().GetOptDebug() && m_lbl_FPS->IsVisible())
	{
		m_lbl_plrPos->Hide();
		m_lbl_plrSpd->Hide();
		m_lbl_plrAcc->Hide();
		m_lbl_mousePos->Hide();
		m_lbl_FPS->Hide();
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
	m_lbl_health->SetMsg(ss.str());
	ss.str("");
	//Energie
	ss << (int)val.Character->Energy() << " / " << (int)val.Character->EnergyMax();
	m_lbl_energy->SetMsg(ss.str());
	ss.str("");
	//Mana
	ss << (int)val.Character->Mana() << " / " << (int)val.Character->ManaMax();
	m_lbl_mana->SetMsg(ss.str());
	ss.str("");
	//Experience
	ss << (int)val.Character->Exp() << " / " << (int)val.Character->ExpNext();
	m_lbl_exp->SetMsg(ss.str());
	ss.str("");
	//Niveau
	ss << val.Character->Level();
	m_lbl_playerLevel->SetMsg(ss.str());
	ss.str("");
	//Position
	ss << "Position :     ( " << std::setprecision(4) << val.Player->Position().x << ", " << std::setprecision(4) <<
		val.Player->Position().y << ", " << std::setprecision(4) << val.Player->Position().z << " )";
	m_lbl_plrPos->SetMsg(ss.str());
	ss.str("");
	//Vitesse
	ss << "Vitesse :      " << val.Player->Speed();
	m_lbl_plrSpd->SetMsg(ss.str());
	ss.str("");
	//Accélération
	ss << "Acceleration : " << val.Player->Acceleration();
	m_lbl_plrAcc->SetMsg(ss.str());
	ss.str("");
	//Psotion souris
	ss << "Pos. Souris :  " << val.Mousepos;
	m_lbl_mousePos->SetMsg(ss.str());
	ss.str("");
	//FPS
	ss << "Fps :          " << std::setprecision(2) << val.Fps;
	m_lbl_FPS->SetMsg(ss.str());
	ss.str("");
	//Heure
	time_t currentTime;
	time (&currentTime);
	struct tm ptm;
	localtime_s(&ptm, &currentTime);
	ss << std::setfill('0');
	ss << std::setw(2) << ptm.tm_hour << ":" << std::setw(2) << ptm.tm_min;
	m_lbl_time->SetMsg(ss.str());
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
	m_pnl_screen->InitControl("main");
	m_pnl_screen->InitLocalizable(Point(), Size(val.Width, val.Height), 0, 0);

#pragma region Enfants de Main

	// Zone de jeu
	m_pnl_playscreen = new Panel();
	m_pnl_playscreen->InitControl("playscreen");
	m_pnl_playscreen->InitLocalizable(Point(INTERFACE_SIDE_LEFT_WIDTH, INTERFACE_BOTTOM_HEIGHT),
		Size(m_pnl_screen->GetSize().w - INTERFACE_SIDE_LEFT_WIDTH - INTERFACE_SIDE_RIGHT_WIDTH, 
		m_pnl_screen->GetSize().h - INTERFACE_BOTTOM_HEIGHT - INTERFACE_TOP_HEIGHT), 0, m_pnl_screen);
	m_pnl_playscreen->InitContainer(PNL_PLAYSCREEN_CONTROLS_NBR);
	m_pnl_screen->AddControl(m_pnl_playscreen);

#pragma region Enfants de Playscreen

	// Informations
	m_lb_infos = new ListBox();
	m_lb_infos->InitControl("lb_infos");
	m_lb_infos->InitLocalizable(Point(5, m_pnl_playscreen->GetSize().h - LBL_GENERIC_CHAR_H * 6 - 26 * (2 + 12)), Size(), 0, m_pnl_playscreen);
	m_lb_infos->InitTextual(val.FontTextures[TEXTCOLOR_RED], false, 12.f, 12.f, 0.5f);
	m_lb_infos->InitListBox(26, 2, Point(), false);
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
	m_lb_console->InitControl(LB_CONSOLE_NAME);
	m_lb_console->InitLocalizable(Point(m_pnl_playscreen->GetSize().w - 64 - LB_CONSOLE_SIZE_W, TXB_CONSOLE_SIZE_H + 5), 
		Size(LB_CONSOLE_SIZE_W, TXB_CONSOLE_SIZE_H), val.UITextures[CUSTIMAGE_CONSOLE_BACK], m_pnl_playscreen);
	m_lb_console->InitTextual(THEME_CONSOLE);
	m_lb_console->InitListBox(LB_CONSOLE_LINE_NUMBER, LB_CONSOLE_LINE_GAP, Point(LB_CONSOLE_BODER_OFFSET_S, LB_CONSOLE_BODER_OFFSET_B),
		true, val.UITextures[CUSTIMAGE_ARROWBUTTON_UP], val.UITextures[CUSTIMAGE_ARROWBUTTON_DOWN]);
	m_pnl_playscreen->AddControl(m_lb_console);
	Info::Get().SetConsole(m_lb_console);
	//Texbox de la console
	m_txb_console = new Textbox();
	m_txb_console->InitControl(TXB_CONSOLE_NAME);
	m_txb_console->InitLocalizable(Point(m_pnl_playscreen->GetSize().w - 64 - LB_CONSOLE_SIZE_W, 0),
		Size(TXB_CONSOLE_SIZE_W, 0), val.UITextures[CUSTIMAGE_CONSOLE_TEXTBOX_BACK], m_pnl_playscreen);
	m_txb_console->InitTextual(THEME_CONSOLE);
	m_txb_console->SetColor(val.FontTextures[TEXTCOLOR_WHITE]);
	m_txb_console->SetOffset(Point(TXB_CONSOLE_OFFSET_X, TXB_CONSOLE_OFFSET_Y));
	m_pnl_playscreen->AddControl(m_txb_console);
	m_txb_console->Hide();
	// Frame portrait
	m_pnl_portrait = new Panel();
	m_pnl_portrait->InitControl(PNL_PORTRAIT_NAME);
	m_pnl_portrait->InitLocalizable(Point(PNL_PORTRAIT_POSITION_X, PNL_PORTRAIT_POSITION_Y),
		Size(PNL_PORTRAIT_SIZE_W, PNL_PORTRAIT_SIZE_H), val.UITextures[CUSTIMAGE_PORTRAIT_FRAME], m_pnl_playscreen);
	m_pnl_portrait->InitContainer(PNL_PORTRAIT_CONTROLS_NBR);
	m_pnl_playscreen->AddControl(m_pnl_portrait);

#pragma region Enfants pnl portrait

	// Barre de vie
	m_pgb_health = new ProgressBar();
	m_pgb_health->InitControl(PGB_HEALTH_NAME);
	m_pgb_health->InitLocalizable(Point(PGB_HEALTH_POSITION_X, PGB_HEALTH_POSITION_Y),
		Size(PGB_HEALTH_SIZE_W, PGB_HEALTH_SIZE_H), val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH_BACK], m_pnl_portrait);
	m_pgb_health->Init(ProgressBar::BARMODE_HORIZONTAL_LTR, val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH], PGB_HEALTH_BORDER_SIZE);
	m_pnl_portrait->AddControl(m_pgb_health);
	// Barre de mana
	m_pgb_mana = new ProgressBar();
	m_pgb_mana->InitControl(PGB_MANA_NAME);
	m_pgb_mana->InitLocalizable(Point(PGB_MANA_POSITION_X, PGB_MANA_POSITION_Y),
		Size(PGB_MANA_SIZE_W, PGB_MANA_SIZE_H), val.UITextures[CUSTIMAGE_PGBTEXT_MANA_BACK], m_pnl_portrait);
	m_pgb_mana->Init(ProgressBar::BARMODE_HORIZONTAL_LTR, val.UITextures[CUSTIMAGE_PGBTEXT_MANA], PGB_MANA_BORDER_SIZE);
	m_pnl_portrait->AddControl(m_pgb_mana);
	// Barre d'expérience
	m_pgb_exp = new ProgressBar();
	m_pgb_exp->InitControl(PGB_EXP_NAME);
	m_pgb_exp->InitLocalizable(Point(PGB_EXP_POSITION_X, PGB_EXP_POSITION_Y),
		Size(PGB_EXP_SIZE_W, PGB_EXP_SIZE_H), val.UITextures[CUSTIMAGE_PGBTEXT_EXP_BACK], m_pnl_portrait);
	m_pgb_exp->Init(ProgressBar::BARMODE_HORIZONTAL_LTR, val.UITextures[CUSTIMAGE_PGBTEXT_EXP], PGB_EXP_BORDER_SIZE);
	m_pnl_portrait->AddControl(m_pgb_exp);
	// Label de vie
	m_lbl_health = new Label();
	m_lbl_health->InitControl(LBL_HEALTH_NAME);
	m_lbl_health->InitLocalizable(Point(LBL_HEALTH_POSITION_X, LBL_HEALTH_POSITION_Y), m_pnl_portrait);
	m_lbl_health->InitTextual(THEME_PLAYER_VALUES);
	m_lbl_health->SetColor(val.FontTextures[TEXTCOLOR_RED]);
	m_pnl_portrait->AddControl(m_lbl_health);
	// Label de mana
	m_lbl_mana = new Label();
	m_lbl_mana->InitControl(LBL_MANA_NAME);
	m_lbl_mana->InitLocalizable(Point(LBL_MANA_POSITION_X, LBL_MANA_POSITION_Y), m_pnl_portrait);
	m_lbl_mana->InitTextual(THEME_PLAYER_VALUES);
	m_lbl_mana->SetColor(val.FontTextures[TEXTCOLOR_BLUE]);
	m_pnl_portrait->AddControl(m_lbl_mana);
	// Label d'exp
	m_lbl_exp = new Label();
	m_lbl_exp->InitControl(LBL_EXP_NAME);
	m_lbl_exp->InitLocalizable(Point(LBL_EXP_POSITION_X, LBL_EXP_POSITION_Y), m_pnl_portrait);
	m_lbl_exp->InitTextual(THEME_PLAYER_VALUES);
	m_lbl_exp->SetColor(val.FontTextures[TEXTCOLOR_YELLOW]);
	m_pnl_portrait->AddControl(m_lbl_exp);

	// Image du joueur
	m_pnl_playerImage = new PictureBox();
	m_pnl_playerImage->InitControl(PB_PORTRAIT_NAME);
	m_pnl_playerImage->InitLocalizable(Point(PB_PORTRAIT_POSITION_X, PB_PORTRAIT_POSITION_Y),
		Size(PB_PORTRAIT_SIZE_W, PB_PORTRAIT_SIZE_H), val.UITextures[CUSTIMAGE_PORTRAIT_MALE], m_pnl_portrait);
	m_pnl_portrait->AddControl(m_pnl_playerImage);

#pragma region Enfants de m_pnl_playerImage

	m_lbl_playerLevel = new Label();
	m_lbl_playerLevel->InitControl(LBL_PLAYER_LEVEL_NAME);
	m_lbl_playerLevel->InitLocalizable(Point(LBL_EXP_POSITION_X, LBL_EXP_POSITION_Y), m_pnl_playerImage);
	m_lbl_playerLevel->InitTextual(val.FontTextures[TEXTCOLOR_YELLOW], false, LBL_PLAYER_LEVEL_H, LBL_PLAYER_LEVEL_W, LBL_PLAYER_LEVEL_I);
	m_lbl_playerLevel->Init(TEXTDOCK_TOPLEFT, Point(20, -5));
	m_pnl_portrait->AddControl(m_lbl_playerLevel);

#pragma endregion

	m_lbl_currentBlockType = new Label();
	m_lbl_currentBlockType->InitControl("lbl_blocinfo");
	m_lbl_currentBlockType->InitLocalizable(m_pnl_playerImage->GetPosition() + Point(0, 200), m_pnl_playscreen);
	m_lbl_currentBlockType->InitTextual(val.FontTextures[TEXTCOLOR_BLUE], false, 
		LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_lbl_currentBlockType->SetMsg("Bloc : ");
	m_pnl_playscreen->AddControl(m_lbl_currentBlockType);

	m_pb_currentBlockType = new PictureBox();
	m_pb_currentBlockType->InitControl("pbcurbloc");
	m_pb_currentBlockType->InitLocalizable(Point(m_lbl_currentBlockType->GetPosition().x + 84, 
		m_lbl_currentBlockType->GetPosition().y), Size(40,40), 0, m_pnl_playscreen);
	m_pnl_playscreen->AddControl(m_pb_currentBlockType);

#pragma endregion

	//Barre d'énergie verticale
	m_pgb_energy = new ProgressBar();
	m_pgb_energy->InitControl(PGB_ENERGY_NAME);
	m_pgb_energy->InitLocalizable(Point(PGB_ENERGY_POSITION_X, PGB_ENERGY_POSITION_Y),
		Size(PGB_ENERGY_SIZE_W, PGB_ENERGY_SIZE_H), val.UITextures[CUSTIMAGE_PGBTEXT_ENERGY_BACK], m_pnl_playscreen);
	m_pgb_energy->Init(ProgressBar::BARMODE_VERTICAL_DTU, val.UITextures[CUSTIMAGE_PGBTEXT_ENERGY], PGB_ENERGY_BORDER_SIZE);
	m_pnl_playscreen->AddControl(m_pgb_energy);
	//Label d'énergie
	m_lbl_energy = new Label();
	m_lbl_energy->InitControl(LBL_ENERGY_NAME);
	m_lbl_energy->InitLocalizable(Point(LBL_ENERGY_POSITION_X, LBL_ENERGY_POSITION_Y), m_pnl_playscreen);
	m_lbl_energy->InitTextual(val.FontTextures[TEXTCOLOR_GREEN], LBL_ENERGY_ITALIC, LBL_ENERGY_CHAR_H, LBL_ENERGY_CHAR_W, LBL_ENERGY_CHAR_I);
	m_pnl_playscreen->AddControl(m_lbl_energy);

#pragma region Controles de debug

	//Label Position
	m_lbl_plrPos = new Label();
	m_lbl_plrPos->InitControl("pos");
	m_lbl_plrPos->InitLocalizable(Point(5, m_pnl_playscreen->GetSize().h - LBL_GENERIC_CHAR_H), m_pnl_playscreen);
	m_lbl_plrPos->InitTextual(val.FontTextures[TEXTCOLOR_GREEN], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_pnl_playscreen->AddControl(m_lbl_plrPos);
	//Label Vitesse
	m_lbl_plrSpd = new Label();
	m_lbl_plrSpd->InitControl("spd");
	m_lbl_plrSpd->InitLocalizable(Point(5, m_pnl_playscreen->GetSize().h - LBL_GENERIC_CHAR_H * 2), m_pnl_playscreen);
	m_lbl_plrSpd->InitTextual(val.FontTextures[TEXTCOLOR_BLUE], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_pnl_playscreen->AddControl(m_lbl_plrSpd);
	//Label Acceleration
	m_lbl_plrAcc = new Label();
	m_lbl_plrAcc->InitControl("acc");
	m_lbl_plrAcc->InitLocalizable(Point(5, m_pnl_playscreen->GetSize().h - LBL_GENERIC_CHAR_H * 3), m_pnl_playscreen);
	m_lbl_plrAcc->InitTextual(val.FontTextures[TEXTCOLOR_RED], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_pnl_playscreen->AddControl(m_lbl_plrAcc);
	//Label mouse position
	m_lbl_mousePos = new Label();
	m_lbl_mousePos->InitControl("mpos");
	m_lbl_mousePos->InitLocalizable(Point(5, m_pnl_playscreen->GetSize().h - LBL_GENERIC_CHAR_H * 4), m_pnl_playscreen);
	m_lbl_mousePos->InitTextual(val.FontTextures[TEXTCOLOR_WHITE], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_pnl_playscreen->AddControl(m_lbl_mousePos);
	//Label FPS
	m_lbl_FPS = new Label();
	m_lbl_FPS->InitControl("fps");
	m_lbl_FPS->InitLocalizable(Point(5, m_pnl_playscreen->GetSize().h - LBL_GENERIC_CHAR_H * 5), m_pnl_playscreen);
	m_lbl_FPS->InitTextual(val.FontTextures[TEXTCOLOR_YELLOW], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_pnl_playscreen->AddControl(m_lbl_FPS);

#pragma endregion

	//Heure
	m_pnl_time = new Panel();
	m_pnl_time->InitControl("clock");
	m_pnl_time->InitLocalizable(Point(m_pnl_playscreen->GetSize().w - 128, m_pnl_playscreen->GetSize().h - 64), 
		Size(128, 64), val.UITextures[CUSTIMAGE_CLOCK_BG], m_pnl_playscreen);
	m_pnl_time->InitContainer(1);
	m_pnl_playscreen->AddControl(m_pnl_time);

#pragma region Enfants de m_pnl_time

	m_lbl_time = new Label();
	m_lbl_time->InitControl("time");
	m_lbl_time->InitLocalizable(Point(), m_pnl_time);
	m_lbl_time->InitTextual(THEME_DEFAULT);
	m_lbl_time->Init(TEXTDOCK_MIDDLECENTER);
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
	Point& pos = m_lb_console->AbsolutePosition();
	Size& size = m_lb_console->GetSize();
	Size& play = m_pnl_screen->GetSize();
	switch (button)
	{
	case MOUSE_BUTTON_RIGHT:
		break;

	case MOUSE_BUTTON_LEFT:
		if (m_lb_console->MousePressEvents(x, m_pnl_playscreen->GetSize().h - y))
			return true;
		break;

	case MOUSE_BUTTON_WHEEL_UP:
		if (x >= pos.x && x <= pos.x + size.w && play.h - y <= pos.y + size.w && play.h - y >= pos.y) {
			m_lb_console->Scroll(1);
			return true;
		} 
		break;

	case MOUSE_BUTTON_WHEEL_DOWN:
		if (x >= pos.x && x <= pos.x + size.w && play.h - y <= pos.y + size.w && play.h - y >= pos.y) {
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
	m_txb_console->Update(val);
	return true;
}

bool GameInterface::KeyPressEvent( unsigned char key )
{
	if (m_txb_console->HasFocus())
		return true;

	return false;
}
