#ifndef INFO_H__
#define INFO_H__

#include "blockinfo.h"
#include "define.h"
#include "array2d.h"
#include "chunk.h"
#include "son.h"
#include "network.h"

class Chunk;

/**
 * @brief Singleton qui contient tout les informations g�n�rales du jeu
 * 
 * Initialise les donn�es dans le constructeur
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
	 * Retourne une r�f�rence vers l'instance de son
	 */
	Son& Sound();

	/**
	 * Retourne une r�f�rence vers l'instance de r�seau
	 */
	Nwork& Network();

	/**
	 * Retourne le bloc info pour le type donn�
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
	 * Retourne le bloc � la position donn� en 
	 * consid�rent un offset
	 * 
	 * @param pos position
	 * @param offset d�calage
	 * @return type du bloc � la position
	 */
	BlockType GetBlocFromWorld(Vector3f pos, const Vector3f& offset = Vector3f(0,0,0)) const;

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
	Nwork m_network;
};

#endif

