#ifndef ENGINE_H__
#define ENGINE_H__
#include "define.h"
#include "openglcontext.h"
#include "texture.h"
#include "Player.h"

class Engine : public OpenglContext
{
public:
	/**
	* DESCRIPTION
	*
	* Constructeur par défaut de la classe
	*
	*/
	Engine();
	/**
	* DESCRIPTION
	*
	* Destructeur par défaut de la classe
	*
	*/
	virtual ~Engine();
	/**
	* DESCRIPTION
	*
	* Initialisation des composantes du jeu
	*
	*/
	virtual void Init();
	virtual void DeInit();
	/**
	* DESCRIPTION
	*
	* Chargement des ressources en mémoire
	*
	*/
	virtual void LoadResource();
	virtual void UnloadResource();
	/**
	* DESCRIPTION
	*
	* Render tous les vertex du jeu
	*
	* @param elapsedTime Temps en millisecondes depuis la dernier
	* tour de boucle
	*
	*/
	virtual void Render(float elapsedTime);
	/**
	* DESCRIPTION
	*
	* Évènement appelé lorsqu'une touche du clavier est enfoncée
	*
	* @param key Touche qui a déclenché l'évènement
	*
	*/
	virtual void KeyPressEvent(unsigned char key);
	/**
	* DESCRIPTION
	*
	* Évènement appelé lorsqu'une touche du clavier est relâchée
	*
	* @param key Touche qui a déclenché l'évènement
	*
	*/
	virtual void KeyReleaseEvent(unsigned char key);
	/**
	* DESCRIPTION
	*
	* Évènement appelé lorsque la position de la souris est modifiée
	*
	* @param x Position en X de la souris au moment du déclenchement de l'évènement
	* @param y Position en Y de la souris au moment du déclenchement de l'évènement
	*
	*/
	virtual void MouseMoveEvent(int x, int y);
	/**
	* DESCRIPTION
	*
	* Évènement appelé lorsqu'une touche de la souris est enfoncée
	*
	* @param button Bouton qui a déclenché l'évènement
	* @param x Coordonnée en X de l'endroit d'où l'évènement a été déclenché
	* @param y Coordonnée en Y de l'endroit d'où l'évènement a été déclenché
	*
	*/
	virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
	/**
	* DESCRIPTION
	*
	* Évènement appelé lorsqu'une touche de la souris est relâchée
	*
	* @param button Bouton qui a déclenché l'évènement
	* @param x Coordonnée en X de l'endroit d'où l'évènement a été déclenché
	* @param y Coordonnée en Y de l'endroit d'où l'évènement a été déclenché
	*
	*/
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
	bool m_run;
	bool m_ghostMode;
};

#endif // ENGINE_H__
