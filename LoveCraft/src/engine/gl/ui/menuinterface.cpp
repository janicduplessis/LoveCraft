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
	m_menu_screen->CtrlInit(0, Vector2i(), Vector2i(val.Width, val.Height), val.UITextures[CUSTIMAGE_MENU_BACKGROUND], "menu_main");
	m_menu_screen->ContainInit(1);
	m_menu_screen->Init();

	//Loading screen
	m_menu_loading = new PictureBox(0, Vector2i(0, 0), m_menu_screen->GP(PictureBox::PROPVCT2_SIZE), val.UITextures[CUSTIMAGE_LOADING_SCREEN], "loading");
	m_menu_loading->SP(Control::PROPBOL_VISIBLE, false);

	//Paneau principal du menu
	m_menu_panel = new Panel();
	m_menu_panel->CtrlInit(m_menu_screen, 
		Vector2i(val.Width / 2 - MENU_PANEL_SIZE_X / 2, val.Height / 2 - MENU_PANEL_SIZE_Y / 2), 
		Vector2i(MENU_PANEL_SIZE_X, MENU_PANEL_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_MAIN_WINDOW], MENU_PANEL_NAME);
	m_menu_panel->ContainInit(2);
	m_menu_panel->Init();
	m_menu_screen->AddControl(m_menu_panel);

	int controlWidth = m_menu_panel->GP(Panel::PROPVCT2_SIZE).x * 0.8f;
	int controlPosX = m_menu_panel->GP(Panel::PROPVCT2_SIZE).x / 2 - controlWidth / 2;

	//Logo du jeu
	m_menu_logo = new PictureBox(m_menu_panel, 
		Vector2i(controlPosX, m_menu_panel->GP(PictureBox::PROPVCT2_SIZE).y - MENU_LOGO_SIZE_Y - controlPosX),
		Vector2i(controlWidth, MENU_LOGO_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_LOGO], "logo");
	m_menu_panel->AddControl(m_menu_logo);

	//Panneau de controle utilisateur
	m_menu_controls = new Panel();
	m_menu_controls->CtrlInit(m_menu_panel,
		Vector2i(controlPosX, controlPosX),
		Vector2i(controlWidth, MENU_CONTROLS_SIZE_Y),
		val.UITextures[CUSTIMAGE_MENU_MAIN_WINDOW], "menu_controls");
	m_menu_controls->ContainInit(3);
	m_menu_controls->Init();
	m_menu_panel->AddControl(m_menu_controls);

	int buttonWidth = m_menu_controls->GP(Panel::PROPVCT2_SIZE).x * 0.8f;
	int buttonPosX = m_menu_controls->GP(Panel::PROPVCT2_SIZE).x / 2 - buttonWidth / 2;

	//Button demarrer fullscreen
	m_menu_fullscreen = new Button();
	m_menu_fullscreen->CtrlInit(m_menu_controls,
		Vector2i(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 3 - MENU_BUTTONS_INTERVAL),
		Vector2i(buttonWidth, MENU_BUTTONS_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_BUTTON_BACK], MENU_BUTTON_START_FULL_NAME);
	m_menu_fullscreen->TextInit(STRING_BUTTON_NORM_START, val.FontTextures[TEXTCOLOR_YELLOW], false, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_menu_fullscreen->Init();
	m_menu_controls->AddControl(m_menu_fullscreen);

	//Button demarrer debug
	m_menu_start = new Button();
	m_menu_start->CtrlInit(m_menu_controls,
		Vector2i(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 2 - MENU_BUTTONS_INTERVAL * 2),
		Vector2i(buttonWidth, MENU_BUTTONS_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_BUTTON_BACK], MENU_BUTTON_DEBUG);
	m_menu_start->TextInit(STRING_BUTTON_DEBUG_START, val.FontTextures[TEXTCOLOR_YELLOW], false, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_menu_start->Init();
	m_menu_controls->AddControl(m_menu_start);

	//Button fermer
	m_menu_close = new Button();
	m_menu_close->CtrlInit(m_menu_controls,
		Vector2i(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 2 - MENU_BUTTONS_INTERVAL * 3),
		Vector2i(buttonWidth, MENU_BUTTONS_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_BUTTON_BACK], MENU_BUTTON_CLOSE);
	m_menu_close->TextInit(STRING_BUTTON_CLOSE, val.FontTextures[TEXTCOLOR_YELLOW], false, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
	m_menu_close->Init();
	m_menu_controls->AddControl(m_menu_close);

	m_loaded = true;
}

void MenuInterface::Render()
{
	m_menu_screen->Render();
}