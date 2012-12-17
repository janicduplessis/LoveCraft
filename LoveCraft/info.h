#ifndef INFO_H__
#define INFO_H__

#include "blockinfo.h"
#include "define.h"

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
	 * Retourne le bloc info pour le type donné
	 * 
	 * @param type type
	 * @return BlocInfo
	 */
	BlockInfo* GetBlocInfo(BlockType type);

private:
	Info();
	~Info();
	Info(Info const& copy);
	Info& operator=(Info const& copy); 
	void GenerateBlocInfos();

private:
	static Info* m_info;

	BlockInfo* m_blocInfos[BTYPE_COUNT];
};

#endif

