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

#include <util/vector3.h>
#include <util/array2d.h>

class Chunk;

/**
* @brief Singleton qui contient tout les informations générales du jeu
* 
* Initialise les données dans le constructeur
*/
class Info
{
public:
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
	void SetChunkArray(Array2d<Chunk>* arrayPtr);

	/**
	* Retourne un pointeur vers l'array 2d de chunks
	*/
	Array2d<Chunk>* GetChunkArray() const;

	/**
	* Retourne le bloc à la position donné en 
	* considérent un offset
	* 
	* @param pos position
	* @param offset décalage
	* @return type du bloc à la position
	*/
	BlockType GetBlocFromWorld(Vector3f pos, const Vector3f& offset = Vector3f(0,0,0)) const;

	/**
	 * Pointeur vers la camera
	 */
	Camera* GetCamera() const;
	void SetCamera(Camera* cam);

	void NextPrint(const std::string& text);
	std::string LineToPrint() const;

private:
	Info();
	~Info();
	Info(Info const& copy);
	Info& operator=(Info const& copy); 
	void GenerateBlocInfos();

private:
	Array2d<Chunk>* m_chunks;
	static Info* m_info;
	BlockInfo* m_blocInfos[BTYPE_COUNT];
	Option m_options;
	Son m_sound;
	Nwork m_network;
	Camera* m_cam;
	Controls m_controls;
	std::string m_lineToPrint;
};

#endif

