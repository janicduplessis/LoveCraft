#ifndef INFO_H__
#define INFO_H__

#include "blockinfo.h"
#include "define.h"
#include "array2d.h"
#include "chunk.h"
#include "son.h"

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
	 * Retourne le bloc info pour le type donné
	 * 
	 * @param type type
	 * @return BlocInfo
	 */
	BlockInfo* GetBlocInfo(BlockType type);

	void SetChunkArray(Array2d<Chunk>* arrayPtr);

	Array2d<Chunk>* GetChunkArray() const;

	BlockType GetBlocFromWorld(Vector3f pos);

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
	Son m_sound;
};

#endif

