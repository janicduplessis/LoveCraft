#include "gameinterface.h"
#include <iostream>
#include <iomanip>
#include <sstream>


GameInterface::GameInterface() : Interface()
{
}

GameInterface::~GameInterface()
{
	DeInit();
}

void GameInterface::Update(const ValuesGameInterface& val)
{
#pragma region Proprietes des controles

	// Update le picturebox du current blocktype
	pb_crntBT->SetBackground(val.BlocTextures->GetTexture(
		Info::Get().GetBlocInfo(val.CurrentBlocType)->GetColorTextureIndex(0)));
	//Affiche ou cache les infos s'il y a un changement
	if (lbx_infos->IsVisible() && !Info::Get().Options().GetOptInfos())
		lbx_infos->Hide();
	else if (!lbx_infos->IsVisible() && Info::Get().Options().GetOptInfos())
		lbx_infos->Show();
	//Change la texture de la barre de vie en fonction du %. Ne réassigne la texture que si on en a besoin
	if (val.Character->HealthPerc() <= PGB_HEALTH_LOW_TRESHOLD && pgb_health->IsForeground(val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH]))
		pgb_health->SetForeground(val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH_LOW]);
	else if (val.Character->HealthPerc() > PGB_HEALTH_LOW_TRESHOLD && pgb_health->IsForeground(val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH_LOW]))
		pgb_health->SetForeground(val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH]);
	//Affiche ou cache la barre d'énergie selon la situation
	if (val.Character->Energy() != val.Character->EnergyMax() || Info::Get().Ctrls().Shift())
	{
		pgb_energy->Show();
		lbl_plrEnergy->Show();
	}
	else if (val.Character->Energy() == val.Character->EnergyMax() && !Info::Get().Ctrls().Shift())
	{
		pgb_energy->Hide();
		lbl_plrEnergy->Hide();
	}
	// Controles de debug
	if (Info::Get().Options().GetOptDebug() && !lbl_FPS->IsVisible())
	{
		lbl_plrPos->Show();
		lbl_plrSpd->Show();
		lbl_plrAcc->Show();
		lbl_mousePos->Show();
		lbl_FPS->Show();
	}
	else if (!Info::Get().Options().GetOptDebug() && lbl_FPS->IsVisible())
	{
		lbl_plrPos->Hide();
		lbl_plrSpd->Hide();
		lbl_plrAcc->Hide();
		lbl_mousePos->Hide();
		lbl_FPS->Hide();
	}

#pragma endregion

#pragma region Actualisation des valeurs

	//Actualisation des valeurs des progressbars
	pgb_health->SetValue(val.Character->HealthPerc());
	pgb_energy->SetValue(val.Character->EnergyPerc());
	pgb_mana->SetValue(val.Character->ManaPerc());
	pgb_exp->SetValue(val.Character->ExpPerc());

	//Actualisation du texte dans les différents Label
	TextUpdate(val);

#pragma endregion

#pragma region Ecriture console a partir de l exterieur

	if (Info::Get().LineToPrint() != "")
	{
		lbx_console->AddLine(Info::Get().LineToPrint());
		Info::Get().NextPrint("");
	}

#pragma endregion
}

void GameInterface::TextUpdate(const ValuesGameInterface& val)
{
	std::ostringstream ss;
	//Vie
	ss << (int)val.Character->Health() << " / " << (int)val.Character->HealthMax();
	lbl_plrHealth->SetMsg(ss.str());
	ss.str("");
	//Energie
	ss << (int)val.Character->Energy() << " / " << (int)val.Character->EnergyMax();
	lbl_plrEnergy->SetMsg(ss.str());
	ss.str("");
	//Mana
	ss << (int)val.Character->Mana() << " / " << (int)val.Character->ManaMax();
	lbl_plrMana->SetMsg(ss.str());
	ss.str("");
	//Experience
	ss << (int)val.Character->Exp() << " / " << (int)val.Character->ExpNext();
	lbl_plrExp->SetMsg(ss.str());
	ss.str("");
	//Niveau
	lbl_playerLevel->SetVariableMsg(val.Character->Level());
	//Position
	lbl_plrPos->SetVariableMsg(val.Player->Position());
	//Vitesse
	lbl_plrSpd->SetVariableMsg(val.Player->Speed());
	//Accélération
	lbl_plrAcc->SetVariableMsg(val.Player->Acceleration());
	//Psotion souris
	lbl_mousePos->SetVariableMsg(val.Mousepos);
	//FPS
	lbl_FPS->SetVariableMsg(val.Fps);
	//Heure
	time_t currentTime;
	time (&currentTime);
	struct tm ptm;
	localtime_s(&ptm, &currentTime);
	ss << std::setfill('0');
	ss << std::setw(2) << ptm.tm_hour << ":" << std::setw(2) << ptm.tm_min;
	lbl_time->SetVariableMsg(ss.str());
	ss.str("");
}

void GameInterface::Init(const ValuesGameInterface& val)
{
	// Écran
	pnl_screen = new Panel();
	pnl_screen->InitControl("main");
	pnl_screen->InitLocalizable(Point(), Size(val.Width, val.Height), 0, 0);

#pragma region Enfants de Main

	// Zone de jeu
	pnl_playscreen = new Panel();
	pnl_playscreen->InitControl("playscreen");
	pnl_playscreen->InitLocalizable(Point(INTERFACE_SIDE_LEFT_WIDTH, INTERFACE_BOTTOM_HEIGHT),
		Size(pnl_screen->GetSize().w - INTERFACE_SIDE_LEFT_WIDTH - INTERFACE_SIDE_RIGHT_WIDTH, 
		pnl_screen->GetSize().h - INTERFACE_BOTTOM_HEIGHT - INTERFACE_TOP_HEIGHT), 0, pnl_screen);

#pragma region Enfants de Playscreen

	// Informations
	lbx_infos = new ListBox();
	lbx_infos->InitControl("lb_infos");
	lbx_infos->InitLocalizable(Point(pnl_playscreen->GetSize().w - 250, pnl_playscreen->GetSize().h - 64 - 26 * (2 + 15)), Size(), 0, pnl_playscreen);
	lbx_infos->InitTextual(val.FontTextures[TEXTCOLOR_RED], false, 15.f, 15.f, 0.5f);
	lbx_infos->InitListBox(26, 2, Point(), false);
	lbx_infos->AddLine("Controles Mouvements");
	lbx_infos->AddLine("Avancer:                    W");
	lbx_infos->AddLine("Reculer:                    S");
	lbx_infos->AddLine("Droite:                     D");
	lbx_infos->AddLine("Gauche:                     A");
	lbx_infos->AddLine("Sauter:                     Espace");
	lbx_infos->AddLine("Marcher:                    Ctrl");
	lbx_infos->AddLine("Se pencher:                 C");
	lbx_infos->AddLine("Courir:                     Shift");
	lbx_infos->AddLine("");
	lbx_infos->AddLine("Controles Debug");
	lbx_infos->AddLine("Tirer:                      1");
	lbx_infos->AddLine("Cochon:                     2");
	lbx_infos->AddLine("Teleporter:                 6");
	lbx_infos->AddLine("Remplir Vie:                7");
	lbx_infos->AddLine("Remplir Energie:            8");
	lbx_infos->AddLine("Augmenter Exp:              P");
	lbx_infos->AddLine("");
	lbx_infos->AddLine("Options");
	lbx_infos->AddLine("Wireframe:                  Y");
	lbx_infos->AddLine("Music On/off                O");
	lbx_infos->AddLine("Music Next                  M");
	lbx_infos->AddLine("Aff/Cach Infos:             F9");
	//lbx_infos->AddLine("Fullscreen                  F10");
	lbx_infos->AddLine("Quitter                     Esc");

	//Fenetre de console
	lbx_console = new ListBox();
	lbx_console->InitControl(LB_CONSOLE_NAME);
	lbx_console->InitLocalizable(Point(pnl_playscreen->GetSize().w - 64 - LB_CONSOLE_SIZE_W, TXB_CONSOLE_SIZE_H + 5), 
		Size(LB_CONSOLE_SIZE_W, TXB_CONSOLE_SIZE_H), val.UITextures[CUSTIMAGE_CONSOLE_BACK], pnl_playscreen);
	lbx_console->InitTextual(THEME_CONSOLE);
	lbx_console->InitListBox(LB_CONSOLE_LINE_NUMBER, LB_CONSOLE_LINE_GAP, Point(LB_CONSOLE_BODER_OFFSET_S, LB_CONSOLE_BODER_OFFSET_B),
		true, val.UITextures[CUSTIMAGE_ARROWBUTTON_UP], val.UITextures[CUSTIMAGE_ARROWBUTTON_DOWN]);
	Info::Get().SetConsole(lbx_console);
	//Texbox de la console
	txb_console = new Textbox();
	txb_console->InitControl(TXB_CONSOLE_NAME);
	txb_console->InitLocalizable(Point(pnl_playscreen->GetSize().w - 64 - LB_CONSOLE_SIZE_W, 0),
		Size(TXB_CONSOLE_SIZE_W, 40), val.UITextures[CUSTIMAGE_CONSOLE_TEXTBOX_BACK], pnl_playscreen);
	txb_console->InitTextual(THEME_CONSOLE);
	txb_console->SetDocking(TEXTDOCK_MIDDLELEFT);
	txb_console->SetColor(val.FontTextures[TEXTCOLOR_WHITE]);
	txb_console->SetOffset(Point(TXB_CONSOLE_OFFSET_X, 0));
	txb_console->Hide();
	// Frame portrait
	pnl_portrait = new Panel();
	pnl_portrait->InitControl(PNL_PORTRAIT_NAME);
	pnl_portrait->InitLocalizable(Point(PNL_PORTRAIT_POSITION_X, PNL_PORTRAIT_POSITION_Y),
		Size(PNL_PORTRAIT_SIZE_W, PNL_PORTRAIT_SIZE_H), val.UITextures[CUSTIMAGE_PORTRAIT_FRAME], pnl_playscreen);

#pragma region Enfants pnl portrait

	// Barre de vie
	pgb_health = new ProgressBar();
	pgb_health->InitControl(PGB_HEALTH_NAME);
	pgb_health->InitLocalizable(Point(PGB_HEALTH_POSITION_X, PGB_HEALTH_POSITION_Y),
		Size(PGB_HEALTH_SIZE_W, PGB_HEALTH_SIZE_H), val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH_BACK], pnl_portrait);
	pgb_health->Init(ProgressBar::BARMODE_HORIZONTAL_LTR, val.UITextures[CUSTIMAGE_PGBTEXT_HEALTH], PGB_HEALTH_BORDER_SIZE);
	// Barre de mana
	pgb_mana = new ProgressBar();
	pgb_mana->InitControl(PGB_MANA_NAME);
	pgb_mana->InitLocalizable(Point(PGB_MANA_POSITION_X, PGB_MANA_POSITION_Y),
		Size(PGB_MANA_SIZE_W, PGB_MANA_SIZE_H), val.UITextures[CUSTIMAGE_PGBTEXT_MANA_BACK], pnl_portrait);
	pgb_mana->Init(ProgressBar::BARMODE_HORIZONTAL_LTR, val.UITextures[CUSTIMAGE_PGBTEXT_MANA], PGB_MANA_BORDER_SIZE);
	// Barre d'expérience
	pgb_exp = new ProgressBar();
	pgb_exp->InitControl(PGB_EXP_NAME);
	pgb_exp->InitLocalizable(Point(PGB_EXP_POSITION_X, PGB_EXP_POSITION_Y),
		Size(PGB_EXP_SIZE_W, PGB_EXP_SIZE_H), val.UITextures[CUSTIMAGE_PGBTEXT_EXP_BACK], pnl_portrait);
	pgb_exp->Init(ProgressBar::BARMODE_HORIZONTAL_LTR, val.UITextures[CUSTIMAGE_PGBTEXT_EXP], PGB_EXP_BORDER_SIZE);
	// Label de vie
	lbl_plrHealth = new Label();
	lbl_plrHealth->InitControl(LBL_HEALTH_NAME);
	lbl_plrHealth->InitLocalizable(Point(LBL_HEALTH_POSITION_X, LBL_HEALTH_POSITION_Y), pnl_portrait);
	lbl_plrHealth->InitTextual(THEME_PLAYER_VALUES);
	lbl_plrHealth->SetColor(val.FontTextures[TEXTCOLOR_RED]);
	// Label de mana
	lbl_plrMana = new Label();
	lbl_plrMana->InitControl(LBL_MANA_NAME);
	lbl_plrMana->InitLocalizable(Point(LBL_MANA_POSITION_X, LBL_MANA_POSITION_Y), pnl_portrait);
	lbl_plrMana->InitTextual(THEME_PLAYER_VALUES);
	lbl_plrMana->SetColor(val.FontTextures[TEXTCOLOR_BLUE]);
	// Label d'exp
	lbl_plrExp = new Label();
	lbl_plrExp->InitControl(LBL_EXP_NAME);
	lbl_plrExp->InitLocalizable(Point(LBL_EXP_POSITION_X, LBL_EXP_POSITION_Y), pnl_portrait);
	lbl_plrExp->InitTextual(THEME_PLAYER_VALUES);
	lbl_plrExp->SetColor(val.FontTextures[TEXTCOLOR_YELLOW]);

	// Image du joueur
	pnl_playerImage = new PictureBox();
	pnl_playerImage->InitControl(PB_PORTRAIT_NAME);
	pnl_playerImage->InitLocalizable(Point(PB_PORTRAIT_POSITION_X, PB_PORTRAIT_POSITION_Y),
		Size(PB_PORTRAIT_SIZE_W, PB_PORTRAIT_SIZE_H), val.UITextures[CUSTIMAGE_PORTRAIT_MALE], pnl_portrait);

	lbl_playerLevel = new Label();
	lbl_playerLevel->InitControl(LBL_PLAYER_LEVEL_NAME);
	lbl_playerLevel->InitLocalizable(Point(LBL_EXP_POSITION_X, LBL_EXP_POSITION_Y), pnl_portrait);
	lbl_playerLevel->InitTextual(val.FontTextures[TEXTCOLOR_YELLOW], false, LBL_PLAYER_LEVEL_H, LBL_PLAYER_LEVEL_W, LBL_PLAYER_LEVEL_I);
	lbl_playerLevel->Init(TEXTDOCK_TOPLEFT, Point(0, -5));
	lbl_playerLevel->SetMsg("&var");

	lbl_crntBT = new Label();
	lbl_crntBT->InitControl("lbl_blocinfo");
	lbl_crntBT->InitLocalizable(pnl_playerImage->GetPosition() + Point(50, pnl_playerImage->GetSize().h + 20), pnl_playscreen);
	lbl_crntBT->InitTextual(val.FontTextures[TEXTCOLOR_BLUE], false, 
		LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	lbl_crntBT->SetMsg("Bloc : ");

	pb_crntBT = new PictureBox();
	pb_crntBT->InitControl("pbcurbloc");
	pb_crntBT->InitLocalizable(Point(lbl_crntBT->GetPosition().x + 84, 
		lbl_crntBT->GetPosition().y), Size(60,60), 0, pnl_playscreen);

#pragma endregion

	//Barre d'énergie verticale
	pgb_energy = new ProgressBar();
	pgb_energy->InitControl(PGB_ENERGY_NAME);
	pgb_energy->InitLocalizable(Point(PGB_ENERGY_POSITION_X, PGB_ENERGY_POSITION_Y),
		Size(PGB_ENERGY_SIZE_W, PGB_ENERGY_SIZE_H), val.UITextures[CUSTIMAGE_PGBTEXT_ENERGY_BACK], pnl_playscreen);
	pgb_energy->Init(ProgressBar::BARMODE_VERTICAL_DTU, val.UITextures[CUSTIMAGE_PGBTEXT_ENERGY], PGB_ENERGY_BORDER_SIZE);
	//Label d'énergie
	lbl_plrEnergy = new Label();
	lbl_plrEnergy->InitControl(LBL_ENERGY_NAME);
	lbl_plrEnergy->InitLocalizable(Point(LBL_ENERGY_POSITION_X, LBL_ENERGY_POSITION_Y), pnl_playscreen);
	lbl_plrEnergy->InitTextual(val.FontTextures[TEXTCOLOR_GREEN], LBL_ENERGY_ITALIC, LBL_ENERGY_CHAR_H, LBL_ENERGY_CHAR_W, LBL_ENERGY_CHAR_I);

#pragma region Controles de debug

	//Label Position
	lbl_plrPos = new Label();
	lbl_plrPos->InitControl("pos");
	lbl_plrPos->InitLocalizable(Point(5, pnl_playscreen->GetSize().h - LBL_GENERIC_CHAR_H), pnl_playscreen);
	lbl_plrPos->InitTextual(val.FontTextures[TEXTCOLOR_GREEN], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	lbl_plrPos->SetMsg("Position:         &var");
	//Label Vitesse
	lbl_plrSpd = new Label();
	lbl_plrSpd->InitControl("spd");
	lbl_plrSpd->InitLocalizable(Point(5, pnl_playscreen->GetSize().h - LBL_GENERIC_CHAR_H * 2), pnl_playscreen);
	lbl_plrSpd->InitTextual(val.FontTextures[TEXTCOLOR_BLUE], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	lbl_plrSpd->SetMsg("Vitesse:          &var");
	//Label Acceleration
	lbl_plrAcc = new Label();
	lbl_plrAcc->InitControl("acc");
	lbl_plrAcc->InitLocalizable(Point(5, pnl_playscreen->GetSize().h - LBL_GENERIC_CHAR_H * 3), pnl_playscreen);
	lbl_plrAcc->InitTextual(val.FontTextures[TEXTCOLOR_RED], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	lbl_plrAcc->SetMsg("Acceleration:     &var");
	//Label mouse position
	lbl_mousePos = new Label();
	lbl_mousePos->InitControl("mpos");
	lbl_mousePos->InitLocalizable(Point(5, pnl_playscreen->GetSize().h - LBL_GENERIC_CHAR_H * 4), pnl_playscreen);
	lbl_mousePos->InitTextual(val.FontTextures[TEXTCOLOR_WHITE], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	lbl_mousePos->SetMsg("Pos. souris:      &var");
	//Label FPS
	lbl_FPS = new Label();
	lbl_FPS->InitControl("fps");
	lbl_FPS->InitLocalizable(Point(5, pnl_playscreen->GetSize().h - LBL_GENERIC_CHAR_H * 5), pnl_playscreen);
	lbl_FPS->InitTextual(val.FontTextures[TEXTCOLOR_YELLOW], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	lbl_FPS->SetMsg("                  &var FPS");

	m_controlmousetest = new Label();
	m_controlmousetest->InitControl("lbl_controltest");
	m_controlmousetest->InitLocalizable(Point(5, pnl_playscreen->GetSize().h - LBL_GENERIC_CHAR_H * 6), pnl_playscreen);
	m_controlmousetest->InitTextual(val.FontTextures[TEXTCOLOR_YELLOW], LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_controlmousetest->SetMsg("Vous pointez sur: &var");

#pragma endregion

	//Heure
	pnl_time = new Panel();
	pnl_time->InitControl("clock");
	pnl_time->InitLocalizable(Point(pnl_playscreen->GetSize().w - 128, pnl_playscreen->GetSize().h - 64), 
		Size(128, 64), val.UITextures[CUSTIMAGE_CLOCK_BG], pnl_playscreen);

#pragma region Enfants de pnl_time

	lbl_time = new Label();
	lbl_time->InitControl("time");
	lbl_time->InitLocalizable(Point(), pnl_time);
	lbl_time->InitTextual(THEME_DEFAULT);
	lbl_time->Init(TEXTDOCK_MIDDLECENTER);
	lbl_time->SetMsg("&var");

#pragma endregion

#pragma endregion

#pragma endregion

#pragma endregion

	//Cursor
	pb_cursor = new PictureBox();
	pb_cursor->InitControl("pb_cursor");
	pb_cursor->InitLocalizable(Point(), Size(40, 40), m_textureInterface[CUSTIMAGE_PERSONAL_CURSOR], 0);

	m_loaded = true;
}

void GameInterface::DeInit()
{
	if (IsLoaded())
	{
		delete pnl_screen;
		delete pnl_playscreen;
		delete pnl_portrait;
		delete pgb_health;
		delete pgb_energy;
		delete pgb_mana;
		delete pgb_exp;
		delete lbx_infos;
		delete lbx_console;
		delete txb_console;
		delete pnl_playerImage;
		delete lbl_playerLevel;
		delete lbl_plrPos;
		delete lbl_plrSpd;
		delete lbl_plrAcc;
		delete lbl_FPS;
		delete pnl_time;
		delete lbl_time;
		delete lbl_plrHealth;
		delete lbl_plrMana;
		delete lbl_plrExp;
		delete lbl_plrEnergy;
		delete lbl_crntBT;
		delete pb_crntBT;
	}
	m_loaded = false;
}

void GameInterface::Render()
{
	pnl_playscreen->Render();
	pb_cursor->Render();
}

void GameInterface::Show()
{
	pnl_screen->Show();
}

void GameInterface::Hide()
{
	pnl_screen->Hide();
}

bool GameInterface::MouseMoveEvents(int x, int y)
{
	m_controlmousetest->SetVariableMsg(pnl_playscreen->GetTopControl(x, y));

	pb_cursor->SetPosition(Point(x, y - pb_cursor->GetSize().h));

	return false;
}

bool GameInterface::MousePressEvent( const MOUSE_BUTTON &button, int x, int y )
{
	switch (button)
	{
	case MOUSE_BUTTON_RIGHT:
		break;

	case MOUSE_BUTTON_LEFT:
		if (lbx_console->MousePressEvents(x, y))
			return true;
		break;

	case MOUSE_BUTTON_WHEEL_UP:
		if (lbx_console->IsWithinRange(x, y))
		{
			lbx_console->Scroll(1);
			return true;
		} 
		break;

	case MOUSE_BUTTON_WHEEL_DOWN:
		if (lbx_console->IsWithinRange(x, y))
		{
			lbx_console->Scroll(-1);
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
		lbx_console->MouseReleaseEvents(x, y);
		break;
	}
	return false;
}

void GameInterface::ConsoleWriteLine( const string& line )
{
	lbx_console->AddLine(line);
}

bool GameInterface::KeyReleaseEvent( unsigned char key )
{
	if (txb_console->HasFocus())
		return true;

	return false;
}

bool GameInterface::TextenteredEvent( unsigned int val )
{
	txb_console->Update(val);
	return true;
}

bool GameInterface::KeyPressEvent( unsigned char key )
{
	if (txb_console->HasFocus())
		return true;

	return false;
}
