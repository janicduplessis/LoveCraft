#ifndef INFO_H__
#define INFO_H__

#include "blockinfo.h"
#include "define.h"

/**
 * @brief Singleton qui contient tout les informations g�n�rales du jeu
 * 
 * Initialise les donn�es dans le constructeur
 */
class Info
{
public:
	static Info& Get();
	static Info* GetPtr();
	BlockInfo* GetBlocInfo(BlockType type);

private:
	Info();
	~Info();
	Info(Info const& copy);            // Not Implemented
	Info& operator=(Info const& copy); // Not Implemented
	void GenerateBlocInfos();

private:
	static Info* m_info;

	BlockInfo* m_blocInfos[BTYPE_COUNT];
};

#endif

