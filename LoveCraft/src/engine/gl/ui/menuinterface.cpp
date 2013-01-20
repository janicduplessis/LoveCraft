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
	m_menu_screen = new Panel(0, Vector2i(), Vector2i(val.Width, val.Height), val.UITextures[CUSTIMAGE_MENU_BACKGROUND], 1, "menu_main");
	m_menu_screen->SetProperty(Control::PROPBOL_REPEATTEXTURE, false);

	//Loading screen
	m_menu_loading = new PictureBox(0, Vector2i(0, 0), m_menu_screen->GetProperty(PictureBox::PROPVCT2_SIZE), val.UITextures[CUSTIMAGE_LOADING_SCREEN], "loading");
	m_menu_loading->SetProperty(Control::PROPBOL_VISIBLE, false);
	m_menu_loading->SetProperty(Control::PROPBOL_REPEATTEXTURE, false);

	//Paneau principal du menu
	m_menu_panel = new Panel(m_menu_screen, 
		Vector2i(val.Width / 2 - MENU_PANEL_SIZE_X / 2, val.Height / 2 - MENU_PANEL_SIZE_Y / 2), 
		Vector2i(MENU_PANEL_SIZE_X, MENU_PANEL_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_MAIN_WINDOW], 2, MENU_PANEL_NAME);
	m_menu_screen->AddControl(m_menu_panel);
	m_menu_panel->SetProperty(Control::PROPBOL_REPEATTEXTURE, false);

	int controlWidth = m_menu_panel->GetProperty(Panel::PROPVCT2_SIZE).x * 0.8f;
	int controlPosX = m_menu_panel->GetProperty(Panel::PROPVCT2_SIZE).x / 2 - controlWidth / 2;

	//Logo du jeu
	m_menu_logo = new PictureBox(m_menu_panel, 
		Vector2i(controlPosX, m_menu_panel->GetProperty(PictureBox::PROPVCT2_SIZE).y - MENU_LOGO_SIZE_Y - controlPosX),
		Vector2i(controlWidth, MENU_LOGO_SIZE_Y), val.UITextures[CUSTIMAGE_MENU_LOGO], "logo");
	m_menu_panel->AddControl(m_menu_logo);
	m_menu_logo->SetProperty(Control::PROPBOL_REPEATTEXTURE, false);

	//Panneau de controle utilisateur
	m_menu_controls = new Panel(m_menu_panel,
		Vector2i(controlPosX, controlPosX),
		Vector2i(controlWidth, MENU_CONTROLS_SIZE_Y),
		val.UITextures[CUSTIMAGE_MENU_MAIN_WINDOW], 3, "menu_controls");
	m_menu_panel->AddControl(m_menu_controls);
	m_menu_controls->SetProperty(Control::PROPBOL_REPEATTEXTURE, false);

	int buttonWidth = m_menu_controls->GetProperty(Panel::PROPVCT2_SIZE).x * 0.8f;
	int buttonPosX = m_menu_controls->GetProperty(Panel::PROPVCT2_SIZE).x / 2 - buttonWidth / 2;

	//Button demarrer fullscreen
	m_menu_fullscreen = new Button(m_menu_controls,
		Vector2i(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 3 - MENU_BUTTONS_INTERVAL), Vector2i(buttonWidth, MENU_BUTTONS_SIZE_Y),
		val.UITextures[CUSTIMAGE_MENU_BUTTON_BACK], val.FontTextures[TEXTCOLOR_YELLOW],
		STRING_BUTTON_NORM_START, MENU_BUTTON_START_FULL_NAME);
	m_menu_controls->AddControl(m_menu_fullscreen);
	m_menu_fullscreen->SetProperty(Control::PROPBOL_REPEATTEXTURE, false);

	//Button demarrer debug
	m_menu_start = new Button(m_menu_controls,
		Vector2i(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 2 - MENU_BUTTONS_INTERVAL * 2), Vector2i(buttonWidth, MENU_BUTTONS_SIZE_Y),
		val.UITextures[CUSTIMAGE_MENU_BUTTON_BACK], val.FontTextures[TEXTCOLOR_YELLOW],
		STRING_BUTTON_DEBUG_START, MENU_BUTTON_DEBUG);
	m_menu_controls->AddControl(m_menu_start);
	m_menu_start->SetProperty(Control::PROPBOL_REPEATTEXTURE, false);

	//Button fermer
	m_menu_close = new Button(m_menu_controls,
		Vector2i(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 1 - MENU_BUTTONS_INTERVAL * 3), Vector2i(buttonWidth, MENU_BUTTONS_SIZE_Y),
		val.UITextures[CUSTIMAGE_MENU_BUTTON_BACK], val.FontTextures[TEXTCOLOR_YELLOW],
		STRING_BUTTON_CLOSE, MENU_BUTTON_CLOSE);
	m_menu_controls->AddControl(m_menu_close);
	m_menu_close->SetProperty(Control::PROPBOL_REPEATTEXTURE, false);

	m_loaded = true;
}

void MenuInterface::Render()
{
	m_menu_screen->Render();
}