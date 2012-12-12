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
#include "testprojectile.h"
#include "vector2.h"

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
	* Render un element au niveau de l'interface
	* avec les données spécifiées. La position doit debutee
	* a partir du point en bas a droite de l'element
	*
	* @param position		Vector2<float> Position en X et Y de l'element
	* @param size			Vector2<float> Taille de l'element (largeur, hauteur)
	* @param texture		Texture qui doit être utiliser avec l'element
	* 
	*/
	virtual void RenderSquare(const Vector2<float>& position, const Vector2<float>& size, Texture& texture);

	/**
	* Affiche le texte donné à l'écran 
	* 
	* @param x		coordonnée en X
	* @param y		coordonnée en Y
	* @param t		texte à afficher
	*/
	virtual void PrinText(unsigned int x, unsigned int y, const std::string& t);

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

	//Stockage des nouveaux points pour permettre de render
	//dans la zone de jeu (sprite, images, etc)
	//À utiliser au lieu de Height() et Width() comme
	//point de départ.
	Vector2<float> m_playScreenTopLeft;
	Vector2<float> m_playScreenTopRight;
	Vector2<float> m_playScreenBotRight;
	Vector2<float> m_playScreenBotLeft;
	Vector2<float> m_playScreenSize;

	Texture m_textureFloor;
	Texture m_textureInterface;
	Texture m_textureFont;
	Texture m_textureCrosshair;
	Texture m_textureCthulhu;
	Texture m_textureGhost;

	Player m_player;
	Camera m_camera;
	Array<bool> m_controls;

	Shader m_shader01;
	Shader m_shaderModel;
	Chunk m_testChunk;

	TestProjectile m_projectile;

	bool m_ghostMode;
	bool m_rightClick;
	bool m_leftClick;
};

#endif // ENGINE_H__
