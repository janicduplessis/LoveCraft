#ifndef INFO_H__
#define INFO_H__

#include "define.h"

#include "blockinfo.h"
#include "chunk.h"
#include "son.h"
#include "network.h"
#include "options.h"
#include "camera.h"
#include "controls.h"
#include "gl/shaders/shader.h"

#include "engine/gl/ui/controls/localizable/textual/listbox.h"
#include "engine/gl/ui/theme.h"

#include <util/vector3.h>
#include <util/array2d.h>
#include "util/dice.h"

class Chunk;

/**
* @brief Singleton qui contient tout les informations générales du jeu
* 
* Initialise les données dans le constructeur
*/
class Info
{
public:
	enum LoadedStatus
	{
		LSTATUS_CHARACTER = 0x1,
		LSTATUS_PLAYER = 0x2,
		LSTATUS_CAMERA = 0x4,
		LSTATUS_CHUNK = 0x8,
		LSTATUS_MONSTERS = 0x10,
		LSTATUS_TEXTURE_IMAGE = 0x20,
		LSTATUS_TEXTURE_FONTS = 0x40,
		LSTATUS_SKYBOX = 0x80
	};
	/**
	* Retourne une reference vers l'instance Info
	*/
	static Info& Get();

	/**
	* Retourne un pointeur vers l'instance Info
	*/
	static Info* GetPtr();

	/**
	* Retourne une référence vers l'instance de son
	*/
	Son& Sound();

	/**
	* Retourne une référence vers l'instance des options
	*/
	Option& Options();

	/**
	* Retourne une référence vers l'instance de réseau
	*/
	Nwork& Network();

	Controls& Ctrls();

	ListBox* Console();
	void SetConsole(ListBox* console);

	/**
	* Retourne le bloc info pour le type donné
	* 
	* @param type type
	* @return BlocInfo
	*/
	BlockInfo* GetBlocInfo(BlockType type);

	/**
	* Set le pointeur vers l'array 2d de chunks
	*/
	void SetChunkArray(Array2d<Chunk*>* arrayPtr);

	/**
	* Retourne un pointeur vers l'array 2d de chunks
	*/
	Array2d<Chunk*>* GetChunkArray() const;

	/**
	* Retourne le bloc à la position donné en 
	* considérent un offset
	* 
	* @param pos position
	* @param offset décalage
	* @return type du bloc à la position
	*/
	BlockType GetBlocFromWorld(Vector3f pos, const Vector3f& offset = Vector3f(0,0,0)) const;

	Vector2i GetMouse();
	void SetMouse(Vector2i pos);

	/**
	* Pointeur vers la camera
	*/
	Camera* GetCamera() const;
	void SetCamera(Camera* cam);

	void NextPrint(const std::string& text);
	std::string LineToPrint() const;

	void StatusOn(LoadedStatus status);
	bool GetStatus(LoadedStatus status) const;

	void SetCubeShader(Shader* shader);
	Shader* GetCubeShader();

	void SetOffsetMap(const Vector2i& offset);
	Vector2i GetOffsetMap() const;

	void SetFonts(Texture** colors);
	Texture* GetFontColor(uint8 index) const;

	void SetTexturesInterface(Texture** textures);
	Texture* GetTexture(uint8 index) const;

	Theme* GetTheme(uint8 index) const;
	Dice* Dice() const;

private:
	Info();
	~Info();
	Info(Info const& copy);
	Info& operator=(Info const& copy); 
	void GenerateBlocInfos();
	void InitThemes();
private:
	Texture** m_fontColors;
	Texture** m_textInterface;
	Theme** m_themes;
	Array2d<Chunk*>* m_chunks;
	static Info* m_info;
	BlockInfo** m_blocInfos;
	Option m_options;
	Son m_sound;
	Nwork m_network;
	Camera* m_cam;
	Controls m_controls;
	string m_lineToPrint;
	Vector2i m_mouse;
	ListBox* m_console;
	int m_lstatus;
	Shader* m_shader;
	Vector2i m_offsetMap;
	Dice* m_dice;
};

#endif

