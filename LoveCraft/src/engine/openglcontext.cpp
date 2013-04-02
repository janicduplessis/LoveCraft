#include "openglcontext.h"
#include "define.h"
#include "info.h"

OpenglContext::OpenglContext() : m_maxFps(60), m_fullscreen(false), m_title(""), m_lastFrameTime(0), 
	m_cursor(0), m_menu(true), m_firstOpen(true), m_sentClose(false)
{
}

OpenglContext::~OpenglContext()
{
}

bool OpenglContext::Start(const std::string& title, int width, int height, bool fullscreen)
{
	m_title = title;
	m_fullscreen = fullscreen;
	InitWindow(width, height);

	sf::Clock clock;	

	while (m_app.isOpen())
	{
		GlobalInit();
		LoadGlobalResource();

		// Jeu
		while (!m_menu && !m_sentClose)
		{
			clock.restart();

#pragma region Premier Tour

			static bool preGame = true;
			if (preGame)
			{
				// Instrcutions du premier tour
				GameInit();
				LoadGameResource();
				preGame = false;
				clock.restart();
			}

#pragma endregion

#pragma region Poll Event

			sf::Event Event;
			while (m_app.pollEvent(Event))
			{
				switch(Event.type)
				{
				case sf::Event::Closed:
					m_sentClose = true;
					m_app.close();
					break;
				case sf::Event::Resized:
					glViewport(0, 0, Event.size.width, Event.size.height);
					break;
				case sf::Event::KeyPressed:
					KeyPressEvent(Event.key.code);
					break;
				case sf::Event::KeyReleased:
					KeyReleaseEvent(Event.key.code);
					break;
				case sf::Event::MouseMoved:
					m_mouseArgs.SetPosition(Vector2i(Event.mouseMove.x, Height() - Event.mouseMove.y));
					m_mouseArgs.SetMouseButtons(ConvertMouseButton(Event.mouseButton.button));
					MouseMoveEvent(m_mouseArgs);
					break;
				case sf::Event::TextEntered:
					TextenteredEvent(Event.text.unicode);
					break;
				case sf::Event::MouseButtonPressed:
					m_mouseArgs.SetPosition(Vector2i(Event.mouseButton.x, Height() - Event.mouseButton.y));
					m_mouseArgs.SetMouseButtons(ConvertMouseButton(Event.mouseButton.button));
					MousePressEvent(m_mouseArgs);
					break;
				case sf::Event::MouseButtonReleased:
					m_mouseArgs.SetPosition(Vector2i(Event.mouseButton.x, Height() - Event.mouseButton.y));
					m_mouseArgs.SetMouseButtons(ConvertMouseButton(Event.mouseButton.button));
					MouseReleaseEvent(m_mouseArgs);
					break;
				case sf::Event::MouseWheelMoved:
					m_mouseArgs.SetPosition(Vector2i(Event.mouseButton.x, Height() - Event.mouseButton.y));
					if(Event.mouseWheel.delta > 0) {
						m_mouseArgs.SetMouseButtons(MOUSE_BUTTON_WHEEL_UP);
					}
					else {
						m_mouseArgs.SetMouseButtons(MOUSE_BUTTON_WHEEL_DOWN);
					}
					MousePressEvent(m_mouseArgs);
					break;
				}
			}
#pragma endregion

			m_app.setActive();
			UpdateGame(m_lastFrameTime);
			RenderGame();
			m_app.display();
			m_lastFrameTime = clock.getElapsedTime().asSeconds();
		}

		// Menu
		while(m_menu && !m_sentClose)
		{
			clock.restart();

#pragma region Premier Tour

			static bool preMenu = true;
			if (preMenu)
			{
				// Instrcutions du premier tour
				MenuInit();
				LoadMenuResource();
				preMenu = false;
			}

#pragma endregion

#pragma region Poll Event

			sf::Event Event;
			while (m_app.pollEvent(Event))
			{
				switch(Event.type)
				{
				case sf::Event::Closed:
					m_sentClose = true;
					m_app.close();
					break;
				case sf::Event::Resized:
					glViewport(0, 0, Event.size.width, Event.size.height);
					break;
				case sf::Event::KeyPressed:
					KeyPressEvent(Event.key.code);
					break;
				case sf::Event::KeyReleased:
					KeyReleaseEvent(Event.key.code);
					break;
				case sf::Event::MouseMoved:
					m_mouseArgs.SetPosition(Vector2i(Event.mouseMove.x, Height() - Event.mouseMove.y));
					m_mouseArgs.SetMouseButtons(ConvertMouseButton(Event.mouseButton.button));
					MouseMoveEvent(m_mouseArgs);
					break;
				case sf::Event::TextEntered:
					TextenteredEvent(Event.text.unicode);
					break;
				case sf::Event::MouseButtonPressed:
					m_mouseArgs.SetPosition(Vector2i(Event.mouseButton.x, Height() - Event.mouseButton.y));
					m_mouseArgs.SetMouseButtons(ConvertMouseButton(Event.mouseButton.button));
					MousePressEvent(m_mouseArgs);
					break;
				case sf::Event::MouseButtonReleased:
					m_mouseArgs.SetPosition(Vector2i(Event.mouseButton.x, Height() - Event.mouseButton.y));
					m_mouseArgs.SetMouseButtons(ConvertMouseButton(Event.mouseButton.button));
					MouseReleaseEvent(m_mouseArgs);
					break;
				case sf::Event::MouseWheelMoved:
					m_mouseArgs.SetPosition(Vector2i(Event.mouseButton.x, Height() - Event.mouseButton.y));
					if(Event.mouseWheel.delta > 0) {
						m_mouseArgs.SetMouseButtons(MOUSE_BUTTON_WHEEL_UP);
					}
					else {
						m_mouseArgs.SetMouseButtons(MOUSE_BUTTON_WHEEL_DOWN);
					}
					MousePressEvent(m_mouseArgs);
					break;
				}
			}

#pragma endregion

			m_app.setActive();
			UpdateMenu(m_lastFrameTime);
			RenderMenu();
			m_app.display();
			m_lastFrameTime = clock.getElapsedTime().asSeconds();
		}
	}

	UnloadResource();
	DeInit();

	return true;
}

bool OpenglContext::Stop()
{
	m_app.close();
	m_sentClose = true;
	return true;
}

void OpenglContext::CenterMouse()
{
	sf::Mouse::setPosition(sf::Vector2i(Width() / 2, Height() / 2), m_app);
}

void OpenglContext::ResetMouse()
{
	sf::Mouse::setPosition(m_mousePos, m_app);
}

void OpenglContext::SetMousePos(int x, int y) 
{
	m_mousePos = sf::Vector2i(x, y);
}

bool OpenglContext::MousePosChanged(int x, int y)
{
	return x != m_mousePos.x || y != m_mousePos.y;
}

int OpenglContext::Width() const
{
	return m_app.getSize().x;
}

int OpenglContext::Height() const
{
	return m_app.getSize().y;
}

void OpenglContext::SetMaxFps(int maxFps)
{
	m_maxFps = maxFps;
	m_app.setFramerateLimit(maxFps);
}

int OpenglContext::GetMaxFps() const
{
	return m_maxFps;
}

void OpenglContext::SetFullscreen(bool fullscreen)
{
	if(m_fullscreen == fullscreen)
		return;

	m_fullscreen = !m_fullscreen;

	DeInit();
	InitWindow(Width(), Height());
	MenuInit();
	GameInit();
}

bool OpenglContext::IsFullscreen() const
{
	return m_fullscreen;
}

void OpenglContext::MakeRelativeToCenter(int& x, int& y) const
{
	x = x - (Width() / 2);
	y = y - (Height() / 2);
}

void OpenglContext::MakeRelativeToMouse(int& x, int& y) const
{
	x = x - m_mousePos.x;
	y = y - m_mousePos.y;
}

Point OpenglContext::MousePosition() const
{
	return Point(sf::Mouse::getPosition(m_app).x, Height() - sf::Mouse::getPosition(m_app).y);
}

void OpenglContext::ShowCursor()
{
	m_app.setMouseCursorVisible(true);
}

void OpenglContext::HideCursor()
{
	m_app.setMouseCursorVisible(false);
}

bool OpenglContext::IsMenuOpen() const
{
	return m_menu;
}
void OpenglContext::SetMenuStatus(const bool value)
{
	m_menu = value;
}

void OpenglContext::ActivateFirstRun()
{
	m_firstOpen = false;
}

bool OpenglContext::IsFirstRun() const
{
	return m_firstOpen;
}

void OpenglContext::InitWindow(int width, int height)
{
	m_app.create(sf::VideoMode(width, height, 32), m_title.c_str(), m_fullscreen ? sf::Style::Fullscreen : (sf::Style::Resize|sf::Style::Close), sf::ContextSettings(32, 8, 0));
}

MouseButton OpenglContext::ConvertMouseButton(sf::Mouse::Button button) const
{
	switch(button)
	{
	case sf::Mouse::Left:
		return MOUSE_BUTTON_LEFT;
	case sf::Mouse::Middle:
		return MOUSE_BUTTON_MIDDLE;
	case sf::Mouse::Right:
		return MOUSE_BUTTON_RIGHT;
	default:
		return MOUSE_BUTTON_NONE;
	}
}
