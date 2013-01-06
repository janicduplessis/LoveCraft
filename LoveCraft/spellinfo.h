#ifndef SPELLINFO_H_
#define SPELLINFO_H_

#include "define.h"
#include "engine/gl/texture.h"

class SpellInfo
{
public:
	enum SType
	{
		S_OFFENSIVE,
		S_DEFENSIVE
	};
	SpellInfo();
	~SpellInfo();
	bool EnoughManaToUse(float crtMana) const;
	float GetDamageAmount() const;
private:
	SpellType m_spell;	//Le sort
	Texture* m_btnImg;	//Image qui apparaitra sur le bouton
	float m_dmg;		//Damage maximal du sort
	float m_mastery;	//Poucentage de maitrise du sort.
	SType m_type;		//Type du sort
	short m_manaCost;	//Cout en mana pour utiliser
};

#endif