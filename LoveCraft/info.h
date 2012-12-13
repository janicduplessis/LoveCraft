#ifndef INFO_H__
#define INFO_H__

#include "blockinfo.h"
#include "define.h"

/**
 * Singleton qui contient tout les informations générales du jeu
 */
class Info
{
public:
	static Info Get();
	static Info* GetPtr();
	BlockInfo* GetBlocInfo(BlockType type);
	void Kill();

private:
	Info();
	Info(Info const& copy);            // Not Implemented
	Info& operator=(Info const& copy); // Not Implemented
	void GenerateBlocInfos();

private:
	static Info* m_info;

	BlockInfo* m_blocInfos[BTYPE_COUNT];
};

#endif

