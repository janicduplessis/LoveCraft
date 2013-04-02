#ifndef OPENGLCONTEXT_H__
#define OPENGLCONTEXT_H__

#include "define.h"

#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "util/vector2.h"
#include "mouseeventargs.h"

// Documentation de SFML: http://www.sfml-dev.org/documentation/index-fr.php
class OpenglContext
{
public:
    OpenglContext();
    virtual ~OpenglContext();

	virtual void GlobalInit() = 0;
	virtual void MenuInit() = 0;
    virtual void GameInit() = 0;
    virtual void DeInit() = 0;

	virtual void LoadGlobalResource() = 0;
    virtual void LoadMenuResource() = 0;
	virtual void LoadGameResource() = 0;
    virtual void UnloadResource() = 0;

	virtual void UpdateMenu(float elapsedtime) = 0;
	virtual void RenderMenu() = 0;

	virtual void UpdateGame(float elapsedTime) = 0;
    virtual void RenderGame() = 0;

	virtual void TextenteredEvent(unsigned int val) = 0;
    virtual void KeyPressEvent(unsigned char key) = 0;
    virtual void KeyReleaseEvent(unsigned char key) = 0;
    virtual void MouseMoveEvent(const MouseEventArgs& e) = 0;
    virtual void MousePressEvent(const MouseEventArgs& e) = 0;
    virtual void MouseReleaseEvent(const MouseEventArgs& e) = 0;


    bool Start(const std::string& title, int width, int height, bool fullscreen);
    bool Stop();

    int Width() const;
    int Height() const;

    void SetMaxFps(int maxFps);
    int GetMaxFps() const;

    void SetFullscreen(bool fullscreen);
    bool IsFullscreen() const;

protected:
    void CenterMouse();
	void ResetMouse();
	void SetMousePos(const Vector2i& pos);
	bool MousePosChanged(int x, int y);
    void MakeRelativeToCenter(int& x, int& y) const;
	void MakeRelativeToMouse(int& x, int& y) const;
	Point MousePosition() const;
    void ShowCursor();
    void HideCursor();
	bool IsMenuOpen() const;
	void SetMenuStatus(const bool value);
	void ActivateFirstRun();
	bool IsFirstRun() const;

private:
    void InitWindow(int width, int height);
    MouseButton ConvertMouseButton(sf::Mouse::Button button) const;

private:
    sf::Window	m_app;
    int			m_maxFps;
    bool		m_fullscreen;
	bool		m_menu;
	bool		m_firstOpen;
	bool		m_sentClose;
    std::string m_title;
	float m_lastFrameTime;
	MouseEventArgs m_mouseArgs;
protected:
	Vector2i m_mousePos; // position de la souris pour calculer la position relative
	sf::Sprite* m_cursor;
};

#endif // OPENGLCONTEXT_H__
