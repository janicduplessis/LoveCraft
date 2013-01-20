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
		ANL_GRD_DEER_M,
		ANL_GRD_COYOTE,
		ANL_GRD_DONKEY,
		ANL_GRD_HUSKY,
		ANL_GRD_HYENA,
		ANL_GRD_REDFOX,
		ANL_GRD_SHEEP,
		ANL_GRD_CATBLK,
		ANL_GRD_BISON,
		ANL_GRD_TORTOISE,
		ANL_GRD_ALIGATR,
		ANL_GRD_TRANTULA,
		ANL_GRD_SCORPION,
		ANL_AIR_MONARCH
	};
	Animal(const Vector3f& pos = Vector3f(0));
	~Animal();
	bool Initialized() const;
	virtual void Init(AnimalType type, Player* player = 0);
	virtual void Init(Player* player = 0);
private:
	bool m_initialized;
};

#endif