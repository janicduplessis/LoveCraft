#ifndef ENGINE_H__
#define ENGINE_H__

#include <ctime>
#include <list>

#include "define.h"
#include "openglcontext.h"

#include "gl/texture.h"
#include "gl/shader.h"
#include "gl/texturearray.h"

#include "player.h"
#include "camera.h"
#include "chunk.h"
#include "info.h"
#include "gl/ui/panel.h"
#include "gl/ui/progressbar.h"
#include "gl/ui/label.h"
#include "gl/ui/listbox.h"
#include "gl/ui/picturebox.h"
#include "gl/ui/button.h"

#include "game/pig.h"
#include "game/character.h"
#include "game/spell.h"

#include "util/array2d.h"
#include "util/vector2.h"

/**
 * @brief Engin graphique
 */
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
	* Retourne une reference en lecture seule vers l'engine
	*/
	virtual const Engine& Get() const;

	/**
	* Retourne une reference vers l'engine
	*/
	virtual Engine& Get();

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
	* Affiche le texte donné à l'écran 
	* 
	* @param x		coordonnée en X
	* @param y		coordonnée en Y
	* @param t		texte à afficher
	*/
	virtual void PrintText(unsigned int x, unsigned int y, const std::string& t);

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
	void LoadBlocTexture(BLOCK_TYPE type, std::string path);
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
	virtual void RenderSquare(const Vector2i& position, const Vector2i& size, Texture& texture, bool repeat = true);
	virtual void RenderSpells();
	void TextUpdate();
	void StartBlendPNG(bool value = true) const;
private:
	//CONSOLE.WRITELINE()
	void CW(const std::string& line);
	void OnClick(Control* sender);
	bool m_wireframe;
	float m_angle;
	float m_camRadius;

	TextureArray* m_textureArray;

	Texture* m_textureSpell;
	Texture* m_textureSpellX;
	Texture* m_textureInterface;
	Texture* m_texturefontColor;
	Texture m_texSpell;

	Player m_player;
	Camera m_camera;
	
	Character m_character;
	Pig m_testpig;

	Shader m_shaderModel;
	Shader m_shaderCube;
	Shader m_shaderSpells;

	Array2d<Chunk>* m_chunks;

	Panel m_pnl_screen;
	Panel m_pnl_playscreen;
	Panel m_pnl_portrait;

	Button m_testButton;

	ProgressBar m_pgb_health;
	ProgressBar m_pgb_energy;
	ProgressBar m_pgb_mana;
	ProgressBar m_pgb_exp;

	ListBox* m_lb_infos;
	ListBox* m_lb_console;

	Panel m_pnl_playerImage;
	Label m_lbl_playerLevel;

	Label m_lbl_plrPos;
	Label m_lbl_plrSpd;
	Label m_lbl_plrAcc;
	Label m_lbl_FPS;

	Panel m_pnl_time;
	Label m_lbl_time;

	Label m_lbl_health;
	Label m_lbl_mana;
	Label m_lbl_exp;
	Label m_lbl_energy;

	bool m_ghostMode;
	bool m_rightClick;
	bool m_leftClick;

	float m_fpstmr;
	float m_fps;

	typedef std::list<Spell> SpellList;
	SpellList m_spells;

	time_t m_time;
};

#endif // ENGINE_H__
