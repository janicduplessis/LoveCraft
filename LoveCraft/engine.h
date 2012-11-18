#ifndef ENGINE_H__
#define ENGINE_H__
#include "define.h"
#include "openglcontext.h"
#include "texture.h"
#include "Player.h"

class Engine : public OpenglContext
{
public:
    Engine();
    virtual ~Engine();
    virtual void Init();
    virtual void DeInit();
    virtual void LoadResource();
    virtual void UnloadResource();
    virtual void Render(float elapsedTime);
    virtual void KeyPressEvent(unsigned char key);
    virtual void KeyReleaseEvent(unsigned char key);
    virtual void MouseMoveEvent(int x, int y);
    virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
    virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);

private:
    bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);

private:
    bool m_wireframe;
	float m_angle;

    Texture m_textureFloor;
	Texture m_textureCube;
	Player m_player;

	bool m_dirLeft;
	bool m_dirRight;
	bool m_dirFront;
	bool m_dirBack;
};

#endif // ENGINE_H__
