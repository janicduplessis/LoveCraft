#ifndef ENGINE_H__
#define ENGINE_H__

#include <ctime>
#include <list>

#include "define.h"
#include "openglcontext.h"
#include "enumerators.h"

#include "gl/texture.h"
#include "gl/shaders/shader.h"
#include "gl/shaders/lightingshader.h"
#include "gl/texturearray.h"
#include "gl/skybox.h"

#include "player.h"
#include "chunkloader.h"
#include "camera.h"
#include "chunk.h"
#include "info.h"
#include "gl/ui/valuesgameinterface.h"
#include "gl/ui/gameinterface.h"
#include "gl/ui/menuinterface.h"
#include "gl/ui/debuginterface.h"
#include "gl/ui/controls/localizable/drawable/picturebox.h"
#include "gl/ui/controls/workable/timer.h"

#include "game/animal.h"
#include "game/character.h"
#include "game/spell.h"

#include "util/array2d.h"
#include "util/vector2.h"
#include "util/dice.h"

//#define LOAD_MODELS

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

	virtual void GlobalInit();
	virtual void MenuInit();
	/**
	* Initialisation des composantes du jeu
	*/
	virtual void GameInit();
	virtual void DeInit();

	virtual void LoadGlobalResource();
	/**
	* Chargement des ressources en mémoire
	*/
	virtual void LoadMenuResource();
	virtual void LoadGameResource();
	virtual void UnloadResource();

	virtual void UpdateMenu(float elapsedTime);
	virtual void RenderMenu();

	/**
	* Met à jour toutes les valeurs du jeu (Game loop)
	*
	* @param elapsedTime	Temps en millisecondes depuis le dernier
	*						tour de boucle
	*/
	virtual void UpdateGame(float elapsedTime);
	/**
	* Render tous les vertex du jeu (Game loop)
	*
	* @param elapsedTime	Temps en millisecondes depuis le dernier
	*						tour de boucle
	*/
	virtual void RenderGame();

	virtual void TextenteredEvent(unsigned int val);
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

	void OnClick(Control* sender);
	void GainedFocus(Textbox* sender);
	void LostFocus(Textbox* sender);
	void Timertest_OnTick(Timer* sender);
	void TimerAnimation_OnTick(Timer* sender);

	void GetBlocAtCursor();
	void AddBlock(BlockType type);
	void RemoveBlock();

private:
	bool LoadTexture(Texture& texture, const string& filename, bool stopOnError = true);
	void LoadBlocTexture(BLOCK_TYPE type, BLOCK_FACE faces, string colorMapPath, string normalMapPath = "");

	void CW(const string& line);
	void CWL(const string& line);


private:
	bool m_wireframe;
	float m_angle;
	float m_camRadius;

	Vector3f m_currentBlock;
	Vector3f m_currentFaceNormal;

	TextureArray* m_textureArray;

	Texture* m_textureSpell;
	Texture* m_textureSpellX;
	Texture** m_texturefontColor;
	Texture m_texSpell;
	Texture m_noNormalMap;
	Texture m_normalMap;

	Player* m_player;
	Camera* m_camera;

	Character* m_character;
	Animal** m_monsters;

	Shader m_shaderModel;
	Shader m_shaderCube;
	Shader m_shaderSpells;
	LightingShader m_lightingShader;

	Array2d<Chunk*>* m_chunks;
	Skybox* m_skybox;

	ValuesInterface m_valuesMenuInterface;
	ValuesGameInterface m_valuesGameInterface;

	MenuInterface m_menuUI;
	GameInterface m_gameUI;
	DebugInterface m_debugUI;

	PictureBox* m_pb_cursor;

	bool m_ghostMode;
	bool m_rightClick;
	bool m_leftClick;

	float m_fpstmr;
	float m_fps;
	float m_clickTimer;
	bool m_clickTimerOn;
	Vector2f m_lastRot;
	Matrix4f m_mxWorld;
	Matrix4f m_mxWVP;
	Matrix4f m_mxProjection;

	BlockType m_currentBlockType;

	typedef std::list<Spell> SpellList;
	SpellList m_spells;

	ChunkLoader m_chunkLoader;

	time_t m_time;
	Timer* m_timertest;
	Timer* m_timeranimationplus;
	Timer* m_timeranimationmoins;

	sf::Mutex m_mutex;
};

#endif // ENGINE_H__
