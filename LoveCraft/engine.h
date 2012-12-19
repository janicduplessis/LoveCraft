#ifndef ENGINE_H__
#define ENGINE_H__

#include "define.h"
#include "openglcontext.h"
#include "texture.h"
#include "player.h"
#include "camera.h"
#include "shader.h"
#include "chunk.h"
#include "arraybool.h"
#include "array2d.h"
#include "testprojectile.h"
#include "vector2.h"
#include "info.h"
#include "textureatlas.h"

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
	* Render les elements 2D en premier plan
	* 
	* @param elapsedTime	Temps en millisecondes depuis le dernier
	*						tour de boucle
	*/
	virtual void Render2D(float elapsedTime);

	/**
	* Render un element au niveau de l'interface
	* avec les donn�es sp�cifi�es. La position doit debutee
	* a partir du point en bas a droite de l'element
	*
	* @param position		Vector2<float> Position en X et Y de l'element
	* @param size			Vector2<float> Taille de l'element (largeur, hauteur)
	* @param texture		Texture qui doit �tre utiliser avec l'element
	* 
	*/
	virtual void RenderSquare(const Vector2i& position, const Vector2i& size, Texture& texture);

	/**
	* Affiche le texte donn� � l'�cran 
	* 
	* @param x		coordonn�e en X
	* @param y		coordonn�e en Y
	* @param t		texte � afficher
	*/
	virtual void PrintText(unsigned int x, unsigned int y, const std::string& t);

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
	void LoadBlocTexture(BLOCK_TYPE type, std::string path);
private:
	bool m_wireframe;
	float m_angle;
	float m_camRadius;

	//Stockage des nouveaux points pour permettre de render
	//dans la zone de jeu (sprite, images, etc)
	//� utiliser au lieu de Height() et Width() comme
	//point de d�part.
	Vector2i m_playScreenTopLeft;
	Vector2i m_playScreenTopRight;
	Vector2i m_playScreenBotRight;
	Vector2i m_playScreenBotLeft;
	Vector2i m_playScreenSize;

	TextureAtlas* m_textureAtlas;

	Texture m_textureFloor;
	Texture m_textureInterface;
	Texture m_textureFont;
	Texture m_textureCrosshair;
	Texture m_textureCthulhu;
	Texture m_textureGhost;

	Player m_player;
	Camera m_camera;
	Array<bool> m_controls;

	sf::Music m_music;
	sf::SoundBuffer m_sndJump;
	Array<sf::SoundBuffer> m_sndFootStep;
	sf::SoundBuffer m_sndClick;

	sf::Sound m_sndChan1;
	sf::Sound m_sndChan2;
	
	Shader m_shader01;
	Shader m_shaderModel;

	Array2d<Chunk>* m_chunks;
	Chunk* m_test;

	Chunk m_testChunk;

	TestProjectile m_projectile;

	bool m_ghostMode;
	bool m_rightClick;
	bool m_leftClick;
};

#endif // ENGINE_H__
