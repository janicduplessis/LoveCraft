#ifndef ENGINE_H__
#define ENGINE_H__
#include "define.h"
#include "openglcontext.h"
#include "texture.h"
#include "Player.h"
#include "camera.h"

class Engine : public OpenglContext
{
public:
	/**
	 * Constructeur par d�faut de la classe
	 */
	Engine();

	/**
	 * Destructeur par d�faut de la classe
	 */
	virtual ~Engine();

	/**
	 * Initialisation des composantes du jeu
	 */
	virtual void Init();

	virtual void DeInit();
	/**
	 * Chargement des ressources en m�moire
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
	 * �v�nement appel� lorsqu'une touche du clavier est enfonc�e
	 *
	 * @param key	 Touche qui a d�clench� l'�v�nement
	 */
	virtual void KeyPressEvent(unsigned char key);

	/**
	 * �v�nement appel� lorsqu'une touche du clavier est rel�ch�e
	 *
	 * @param key	Touche qui a d�clench� l'�v�nement
	 */
	virtual void KeyReleaseEvent(unsigned char key);

	/**
	 * �v�nement appel� lorsque la position de la souris est modifi�e
	 *
	 * @param x Position en X de la souris au moment du d�clenchement de l'�v�nement
	 * @param y Position en Y de la souris au moment du d�clenchement de l'�v�nement
	 */
	virtual void MouseMoveEvent(int x, int y);

	/**
	 * �v�nement appel� lorsqu'une touche de la souris est enfonc�e
	 *
	 * @param button Bouton qui a d�clench� l'�v�nement
	 * @param x Coordonn�e en X de l'endroit d'o� l'�v�nement a �t� d�clench�
	 * @param y Coordonn�e en Y de l'endroit d'o� l'�v�nement a �t� d�clench�
	 */
	virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);

	/**
	 * �v�nement appel� lorsqu'une touche de la souris est rel�ch�e
	 *
	 * @param button Bouton qui a d�clench� l'�v�nement
	 * @param x Coordonn�e en X de l'endroit d'o� l'�v�nement a �t� d�clench�
	 * @param y Coordonn�e en Y de l'endroit d'o� l'�v�nement a �t� d�clench�
	 */
	virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);

private:
	bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);

private:
	bool m_wireframe;
	float m_angle;

	Texture m_textureFloor;
	Texture m_textureCube;
	Texture m_textureWall;
	Texture m_textureCeiling;

	Player m_player;
	Camera m_camera;

	bool m_dirLeft;
	bool m_dirRight;
	bool m_dirFront;
	bool m_dirBack;
	bool m_run;
	bool m_ghostMode;
};

#endif // ENGINE_H__
