#include "character.h"


Character::Character()
{
}


Character::~Character()
{
}

Character::Character(std::string name, float health, float energy, float mana)
{

}
std::string Character::Name() const
{
	return m_name;
}
void Character::SetHealth(const float value)
{

}
void Character::SetEnergy(const float value)
{

}
void Character::SetMana(const float value)
{

}

float Character::Health() const
{
	return m_health;
}
float Character::Energy() const
{
	return m_energy;
}
float Character::Mana() const
{
	return m_mana;
}

float Character::HealthMax() const
{
	return m_healthMax;
}
float Character::EnergyMax() const
{
	return m_energyMax;
}
float Character::ManaMax() const
{
	return m_manaMax;
}

float Character::HealthPerc() const
{
	return (m_health / m_healthMax);
}
float Character::EnergyPerc() const
{
	return (m_energy / m_energyMax);
}
float Character::ManaPerc() const
{
	return (m_mana / m_manaMax);
}