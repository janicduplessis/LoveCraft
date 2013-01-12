#ifndef ANIMAL_H_
#define ANIMAL_H_

#include "npc.h"

class Animal : public Npc
{
public:
	enum AnimalType
	{
		ANL_GRD_PIG,
		ANL_GRD_BWIDOW,
		ANL_GRD_ARMDILLO,
		ANL_GRD_RHINO,
		ANL_GRD_ARACHNID,
		ANL_GRN_DEER_M
	};
	Animal();
	~Animal();
	bool Initialized() const;
	virtual void Init(AnimalType type, Player* player = 0);
	virtual void Init(Player* player = 0);
private:
	bool m_initialized;
};

#endif