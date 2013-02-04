#include "menuinterface.h"


MenuInterface::MenuInterface() : Interface()
{
}


MenuInterface::~MenuInterface()
{
	if (m_loaded)
	{
		delete m_menu_close;
		delete m_menu_controls;
		delete m_menu_fullscreen;
		delete m_menu_logo;
		delete m_menu_panel;
		delete m_menu_screen;
		delete m_menu_start;
	}
}

void MenuInterface::Init(const ValuesInterface& val)
{
	//Fond d'écran
	m_menu_screen = new Panel();
	m_menu_screen->InitControl("menu_main");
	m_menu_screen->InitLocalizable(Point(), Size(val.Width, val.Height), val.UITextures[CUSTIMAGE_MENU_BACKGROUND]);

	//Loading screen
	m_menu_loading = new PictureBox();
	m_menu_loading->InitControl("loading", m_menu_screen);
	m_menu_loading->InitLocalizable(Point(), m_menu_screen->GetSize(), val.UITextures[CUSTIMAGE_LOADING_SCREEN]);
	m_menu_loading->Hide();
	m_menu_screen->AddControl(m_menu_loading);

	//Paneau principal du menu
	m_menu_panel = new Panel();
	m_menu_panel->InitControl(MENU_PANEL_NAME, m_menu_screen);
	m_menu_panel->InitLocalizable(Point(val.Width / 2 - MENU_PANEL_SIZE_X / 2, val.Height / 2 - MENU_PANEL_SIZE_Y / 2), 
		Size(MENU_PANEL_SIZE_X, MENU_PANEL_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_MAIN_WINDOW]);
	m_menu_panel->InitContainer(2);
	m_menu_screen->AddControl(m_menu_panel);

	int controlWidth = m_menu_panel->GetSize().w * 0.8f;
	int controlPosX = m_menu_panel->GetSize().w / 2 - controlWidth / 2;

	//Logo du jeu
	m_menu_logo = new PictureBox();
	m_menu_logo->InitControl("logo", m_menu_panel);
	m_menu_logo->InitLocalizable(Point(controlPosX, m_menu_panel->GetSize().h - MENU_LOGO_SIZE_Y - controlPosX),
		Size(controlWidth, MENU_LOGO_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_LOGO]);
	m_menu_panel->AddControl(m_menu_logo);

	//Panneau de controle utilisateur
	m_menu_controls = new Panel();
	m_menu_controls->InitControl("menu_controls", m_menu_panel);
	m_menu_controls->InitLocalizable(Point(controlPosX, controlPosX),
		Size(controlWidth, MENU_CONTROLS_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_MAIN_WINDOW]);
	m_menu_controls->InitContainer(3);
	m_menu_panel->AddControl(m_menu_controls);

	int buttonWidth = m_menu_controls->GetSize().w * 0.8f;
	int buttonPosX = m_menu_controls->GetSize().w / 2 - buttonWidth / 2;

	//Button demarrer fullscreen
	m_menu_fullscreen = new Button();
	m_menu_fullscreen->InitControl(MENU_BUTTON_START_FULL_NAME, m_menu_controls);
	m_menu_fullscreen->InitLocalizable(Point(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 3 - MENU_BUTTONS_INTERVAL),
		Size(buttonWidth, MENU_BUTTONS_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_BUTTON_BACK]);
	m_menu_fullscreen->InitTextual(THEME_MAINMENU);
	m_menu_fullscreen->SetMsg(STRING_BUTTON_NORM_START);
	m_menu_controls->AddControl(m_menu_fullscreen);

	//Button demarrer debug
	m_menu_start = new Button();
	m_menu_start->InitControl(MENU_BUTTON_DEBUG, m_menu_controls);
	m_menu_start->InitLocalizable(Point(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 2 - MENU_BUTTONS_INTERVAL * 2),
		Size(buttonWidth, MENU_BUTTONS_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_BUTTON_BACK]);
	m_menu_start->InitTextual(THEME_MAINMENU);
	m_menu_start->SetMsg(STRING_BUTTON_DEBUG_START);
	m_menu_controls->AddControl(m_menu_start);

	//Button fermer
	m_menu_close = new Button();
	m_menu_close->InitControl(MENU_BUTTON_CLOSE, m_menu_controls);
	m_menu_close->InitLocalizable(Point(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 1 - MENU_BUTTONS_INTERVAL * 3),
		Size(buttonWidth, MENU_BUTTONS_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_BUTTON_BACK]);
	m_menu_close->InitTextual(THEME_MAINMENU);
	m_menu_close->SetMsg(STRING_BUTTON_CLOSE);
	m_menu_controls->AddControl(m_menu_close);


	// controles de test
	m_timertest = new Panel();
	m_timertest->InitControl("timertest", 0);
	m_timertest->InitLocalizable(Point(100, 100), Size(400, 400), val.UITextures[CUSTIMAGE_CONSOLE_BACK]);
	m_menu_screen->AddControl(m_timertest);

	m_timertesttext = new Label();
	m_timertesttext->InitControl("text", m_timertest);
	m_timertesttext->InitLocalizable(Point(100, 300), Size(), 0);
	m_timertesttext->RemoveBackground();
	m_timertesttext->SetCharHeight(24.f);
	m_timertesttext->SetCharWidth(14.f);
	m_timertesttext->SetCharInterval(0.5f);
	m_timertesttext->SetColor(val.FontTextures[TEXTCOLOR_BLUE]);
	m_timertesttext->SetMsg("Appuyez sur espace pour tester.");
	m_timertest->AddControl(m_timertesttext);

	m_loaded = true;
}

void MenuInterface::Render()
{
	m_menu_screen->Render();
}