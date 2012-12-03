#ifndef ENGINE_H__
#define ENGINE_H__
#include "define.h"
#include "openglcontext.h"
#include "texture.h"
#include "Player.h"
#include "camera.h"
#include "shader.h"
#include "Chunk.h"
#include "ArrayBool.h"

class Engine : public OpenglContext
{
public:
	/**
	 * Constructeur par défaut de la classe
	 */
	Engine();

	/**
	 * Destructeur par défaut de la classe
	 */
	virtual ~Engine();

	/**
	 * Initialisation des composantes du jeu
	 */
	virtual void Init();

	virtual void DeInit();
	/**
	 * Chargement des ressources en mémoire
	 */
	virtual void LoadResource();

	virtual void UnloadResource();

	/**
	 * Render tous les vertex du jeu (Game loop)
	 *
	 * @param elapsedTime	Temps en millisecondes depuis le dernier
	 *						tour de boucle
	 */
	virtual void Render(float elapsedTime);

	/**
	 * Render les elements 2D en premier plan
	 * 
	 * @param elapsedTime	Temps en millisecondes depuis le dernier
	 *						tour de boucle
	 */
	virtual void Render2D(float elapsedTime);

	/**
	 * Évènement appelé lorsqu'une touche du clavier est enfoncée
	 *
	 * @param key	 Touche qui a déclenché l'évènement
	 */
	virtual void KeyPressEvent(unsigned char key);

	/**
	 * Évènement appelé lorsqu'une touche du clavier est relâchée
	 *
	 * @param key	Touche qui a déclenché l'évènement
	 */
	virtual void KeyReleaseEvent(unsigned char key);

	/**
	 * Évènement appelé lorsque la position de la souris est modifiée
	 *
	 * @param x Position en X de la souris au moment du déclenchement de l'évènement
	 * @param y Position en Y de la souris au moment du déclenchement de l'évènement
	 */
	virtual void MouseMoveEvent(int x, int y);

	/**
	 * Évènement appelé lorsqu'une touche de la souris est enfoncée
	 *
	 * @param button Bouton qui a déclenché l'évènement
	 * @param x Coordonnée en X de l'endroit d'où l'évènement a été déclenché
	 * @param y Coordonnée en Y de l'endroit d'où l'évènement a été déclenché
	 */
	virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);

	/**
	 * Évènement appelé lorsqu'une touche de la souris est relâchée
	 *
	 * @param button Bouton qui a déclenché l'évènement
	 * @param x Coordonnée en X de l'endroit d'où l'évènement a été déclenché
	 * @param y Coordonnée en Y de l'endroit d'où l'évènement a été déclenché
	 */
	virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);

private:
	bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);

private:
	bool m_wireframe;
	float m_angle;
	float m_camRadius;

	Texture m_textureFloor;

	Player m_player;
	Camera m_camera;
	Array<bool> m_controls;

	Shader m_shader01;
	Chunk m_testChunk;

	bool m_ghostMode;
	bool m_rightClick;
	bool m_leftClick;
};

#endif // ENGINE_H__
