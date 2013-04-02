#include "menuinterface.h"
#include "uiimage.h"
#include "fontcolor.h"

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
	pnl_screen->InitLocalizable(Point(), Size(val.Width, val.Height), CUSTIMAGE_MENU_BACKGROUND, 0);

	//Paneau principal du menu
	pnl_main = new Panel();
	pnl_main->InitControl(MENU_PANEL_NAME);
	pnl_main->InitLocalizable(Point(val.Width / 2 - MENU_PANEL_SIZE_X / 2, val.Height / 2 - MENU_PANEL_SIZE_Y / 2), 
		Size(MENU_PANEL_SIZE_X, MENU_PANEL_SIZE_Y), CUSTIMAGE_MENU_MAIN_WINDOW, pnl_screen);

	int controlWidth = pnl_main->GetSize().w * 0.8f;
	int controlPosX = pnl_main->GetSize().w / 2 - controlWidth / 2;

	//Logo du jeu
	pb_logo = new PictureBox();
	pb_logo->InitControl("pb_logo");
	pb_logo->InitLocalizable(Point(controlPosX, pnl_main->GetSize().h - MENU_LOGO_SIZE_Y - controlPosX),
		Size(controlWidth, MENU_LOGO_SIZE_Y), CUSTIMAGE_MENU_LOGO, pnl_main);

	//Panneau de controle utilisateur
	pnl_controls = new Panel();
	pnl_controls->InitControl("menu_controls");
	pnl_controls->InitLocalizable(Point(controlPosX, controlPosX),
		Size(controlWidth, MENU_CONTROLS_SIZE_Y), CUSTIMAGE_MENU_MAIN_WINDOW, pnl_main);

	int buttonWidth = pnl_controls->GetSize().w * 0.8f;
	int buttonPosX = pnl_controls->GetSize().w / 2 - buttonWidth / 2;

	//Button demarrer fullscreen
	btn_debugStart = new Button();
	btn_debugStart->InitControl(MENU_BUTTON_START_FULL_NAME);
	btn_debugStart->InitLocalizable(Point(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 3 - MENU_BUTTONS_INTERVAL),
		Size(buttonWidth, MENU_BUTTONS_SIZE_Y), CUSTIMAGE_MENU_BUTTON_BACK, pnl_controls);
	btn_debugStart->InitTextual(THEME_MAINMENU);
	btn_debugStart->SetMsg(STRING_BUTTON_NORM_START);

	//Button demarrer debug
	btn_normStart = new Button();
	btn_normStart->InitControl(MENU_BUTTON_DEBUG);
	btn_normStart->InitLocalizable(Point(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 2 - MENU_BUTTONS_INTERVAL * 2),
		Size(buttonWidth, MENU_BUTTONS_SIZE_Y), CUSTIMAGE_MENU_BUTTON_BACK, pnl_controls);
	btn_normStart->InitTextual(THEME_MAINMENU);
	btn_normStart->SetMsg(STRING_BUTTON_DEBUG_START);

	//Button fermer
	btn_close = new Button();
	btn_close->InitControl(MENU_BUTTON_CLOSE);
	btn_close->InitLocalizable(Point(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 1 - MENU_BUTTONS_INTERVAL * 3),
		Size(buttonWidth, MENU_BUTTONS_SIZE_Y), CUSTIMAGE_MENU_BUTTON_BACK, pnl_controls);
	btn_close->InitTextual(THEME_MAINMENU);
	btn_close->SetMsg(STRING_BUTTON_CLOSE);

	//Loading screen
	pnl_loading = new Panel();
	pnl_loading->InitControl("loading");
	pnl_loading->InitLocalizable(Point(), Size(val.Width, val.Height), CUSTIMAGE_LOADING_SCREEN, pnl_screen);
	pnl_loading->Hide();

	//Cursor
	pb_cursor = new PictureBox();
	pb_cursor->InitControl("pb_cursor");
	pb_cursor->InitLocalizable(Point(), Size(40, 40), CUSTIMAGE_PERSONAL_CURSOR, 0);

	m_mousepostest = new Label();
	m_mousepostest->InitControl("mousetest");
	m_mousepostest->InitLocalizable(Point(50, 250), pnl_main);
	m_mousepostest->SetCharHeight(24.f);
	m_mousepostest->SetCharWidth(14.f);
	m_mousepostest->SetCharInterval(0.5f);
	m_mousepostest->SetOffset(Point(0, 70));
	m_mousepostest->SetDocking(TEXTDOCK_MIDDLECENTER);
	m_mousepostest->SetColor(TEXTCOLOR_BLUE);
	m_mousepostest->SetMsg("Est dans le panel principal: &var");

	m_controlmousetest = new Label();
	m_controlmousetest->InitControl("controltest");
	m_controlmousetest->InitLocalizable(Point(50, 250), pnl_main);
	m_controlmousetest->SetCharHeight(24.f);
	m_controlmousetest->SetCharWidth(14.f);
	m_controlmousetest->SetCharInterval(0.5f);
	m_controlmousetest->SetOffset(Point(0, 100));
	m_controlmousetest->SetDocking(TEXTDOCK_MIDDLECENTER);
	m_controlmousetest->SetColor(TEXTCOLOR_BLUE);
	m_controlmousetest->SetMsg("Vous pointez sur: &var");

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
	pb_cursor->Render();
}

void MenuInterface::Show()
{
	pnl_main->Show();
}

void MenuInterface::Hide()
{
	pnl_main->Hide();
}

bool MenuInterface::MouseMoveEvents(const MouseEventArgs& e)
{
	m_mousepostest->SetVariableMsg(pnl_main->MouseMoveEvents(e.GetPosition().x, e.GetPosition().y));
	m_controlmousetest->SetVariableMsg(pnl_screen->GetTopControl(e.GetPosition().x, e.GetPosition().y));
	if (pnl_main->MouseMoveEvents(e.GetPosition().x, e.GetPosition().y))
		pb_logo->Show();
	else pb_logo->Hide();

	pb_cursor->SetPosition(Point(x, y - pb_cursor->GetSize().h));

	return false;
}

bool MenuInterface::MousePressEvents(const MouseButton &button, int x, int y)
{
	switch (button)
	{
	case MOUSE_BUTTON_RIGHT:
		break;

	case MOUSE_BUTTON_LEFT:
		if (btn_close->MousePressEvents(x, y))
			return true;
		if (btn_normStart->MousePressEvents(x, y))
			return true;
		if (btn_debugStart->MousePressEvents(x, y))
			return true;
		break;

	case MOUSE_BUTTON_WHEEL_UP:
		break;

	case MOUSE_BUTTON_WHEEL_DOWN:
		break;
	}
	return false;

}

bool MenuInterface::MouseReleaseEvents(const MouseButton &button, int x, int y)
{
	switch (button)
	{
	case MOUSE_BUTTON_RIGHT:
		break;

	case MOUSE_BUTTON_LEFT:
		if (btn_close->MouseReleaseEvents(x, y))
			return true;
		if (btn_normStart->MouseReleaseEvents(x, y))
			return true;
		if (btn_debugStart->MouseReleaseEvents(x, y))
			return true;
		break;

	case MOUSE_BUTTON_WHEEL_UP:
		break;

	case MOUSE_BUTTON_WHEEL_DOWN:
		break;
	}
	return false;
}