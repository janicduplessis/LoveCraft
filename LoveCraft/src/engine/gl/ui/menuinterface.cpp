#include "menuinterface.h"


MenuInterface::MenuInterface() : Interface()
{
}


MenuInterface::~MenuInterface()
{
	DeInit();
}

void MenuInterface::Init(const ValuesInterface& val)
{
	//Fond d'écran
	pnl_screen = new Panel();
	pnl_screen->InitControl("menu_main");
	pnl_screen->InitLocalizable(Point(), Size(val.Width, val.Height), val.UITextures[CUSTIMAGE_MENU_BACKGROUND], 0);

	//Paneau principal du menu
	pnl_main = new Panel();
	pnl_main->InitControl(MENU_PANEL_NAME);
	pnl_main->InitLocalizable(Point(val.Width / 2 - MENU_PANEL_SIZE_X / 2, val.Height / 2 - MENU_PANEL_SIZE_Y / 2), 
		Size(MENU_PANEL_SIZE_X, MENU_PANEL_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_MAIN_WINDOW], pnl_screen);
	pnl_main->InitContainer(2);
	pnl_screen->AddControl(pnl_main);

	int controlWidth = pnl_main->GetSize().w * 0.8f;
	int controlPosX = pnl_main->GetSize().w / 2 - controlWidth / 2;

	//Logo du jeu
	pb_logo = new PictureBox();
	pb_logo->InitControl("pb_logo");
	pb_logo->InitLocalizable(Point(controlPosX, pnl_main->GetSize().h - MENU_LOGO_SIZE_Y - controlPosX),
		Size(controlWidth, MENU_LOGO_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_LOGO], pnl_main);
	pnl_main->AddControl(pb_logo);

	//Panneau de controle utilisateur
	pnl_controls = new Panel();
	pnl_controls->InitControl("menu_controls");
	pnl_controls->InitLocalizable(Point(controlPosX, controlPosX),
		Size(controlWidth, MENU_CONTROLS_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_MAIN_WINDOW], pnl_main);
	pnl_controls->InitContainer(3);
	pnl_main->AddControl(pnl_controls);

	int buttonWidth = pnl_controls->GetSize().w * 0.8f;
	int buttonPosX = pnl_controls->GetSize().w / 2 - buttonWidth / 2;

	//Button demarrer fullscreen
	btn_debugStart = new Button();
	btn_debugStart->InitControl(MENU_BUTTON_START_FULL_NAME);
	btn_debugStart->InitLocalizable(Point(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 3 - MENU_BUTTONS_INTERVAL),
		Size(buttonWidth, MENU_BUTTONS_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_BUTTON_BACK], pnl_controls);
	btn_debugStart->InitTextual(THEME_MAINMENU);
	btn_debugStart->SetMsg(STRING_BUTTON_NORM_START);
	pnl_controls->AddControl(btn_debugStart);

	//Button demarrer debug
	btn_normStart = new Button();
	btn_normStart->InitControl(MENU_BUTTON_DEBUG);
	btn_normStart->InitLocalizable(Point(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 2 - MENU_BUTTONS_INTERVAL * 2),
		Size(buttonWidth, MENU_BUTTONS_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_BUTTON_BACK], pnl_controls);
	btn_normStart->InitTextual(THEME_MAINMENU);
	btn_normStart->SetMsg(STRING_BUTTON_DEBUG_START);
	pnl_controls->AddControl(btn_normStart);

	//Button fermer
	btn_close = new Button();
	btn_close->InitControl(MENU_BUTTON_CLOSE);
	btn_close->InitLocalizable(Point(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 1 - MENU_BUTTONS_INTERVAL * 3),
		Size(buttonWidth, MENU_BUTTONS_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_BUTTON_BACK], pnl_controls);
	btn_close->InitTextual(THEME_MAINMENU);
	btn_close->SetMsg(STRING_BUTTON_CLOSE);
	pnl_controls->AddControl(btn_close);

	//Loading screen
	pnl_loading = new Panel();
	pnl_loading->InitControl("loading");
	pnl_loading->InitLocalizable(Point(), Size(val.Width, val.Height), val.UITextures[CUSTIMAGE_LOADING_SCREEN], pnl_screen);
	pnl_loading->Hide();
	pnl_screen->AddControl(pnl_loading);

	m_loaded = true;
}

void MenuInterface::DeInit()
{
	if (IsLoaded())
	{
		delete btn_close;
		delete pnl_controls;
		delete btn_debugStart;
		delete pb_logo;
		delete pnl_main;
		delete pnl_screen;
		delete btn_normStart;
	}
	m_loaded = false;
}

void MenuInterface::Render()
{
	pnl_screen->Render();
}