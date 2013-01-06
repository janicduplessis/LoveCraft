#include "spellinfo.h"


SpellInfo::SpellInfo()
{
}


SpellInfo::~SpellInfo()
{
}

bool SpellInfo::EnoughManaToUse(float crtMana) const
{
	return crtMana - m_manaCost >= 0;
}
float SpellInfo::GetDamageAmount() const
{
	//Le damage se calcul en applicant la maitrise au maimum de dmg pour connaitre le range
	//On fait ensuite un random entre les deux et on retourne de résultat
	return 20.0f;
}