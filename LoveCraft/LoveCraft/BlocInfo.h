#ifndef BLOC_INFO
#define BLOC_INFO

#include "define.h"
#include <string>

class BlocInfo
{
public:
	BlocInfo(BlockType type, const std::string& nom);

	BlockType GetType() const;

	void SetDurabilite(int durabilite);
	int GetDurabilite() const;




	~BlocInfo();
};

#endif