#include "character.h"


Character::Character() : m_name("DeFacto"), m_healthMax(100), m_energyMax(150), m_manaMax(50),
	m_health(m_healthMax), m_energy(m_energyMax), m_mana(m_manaMax)
{
}


Character::~Character()
{
}

Character::Character(std::string name, float health, float energy, float mana) : m_name(name), 
	m_healthMax(health), m_energyMax(energy), m_manaMax(mana), m_health(m_healthMax), m_energy(m_energyMax), 
	m_mana(m_manaMax)
{

}
std::string Character::Name() const
{
	return m_name;
}
void Character::SetHealth(const float value)
{
	m_health = value >= m_healthMax ? m_healthMax : (value <= 0 ? 0 : value);
}
void Character::SetEnergy(const float value)
{
	m_energy = value >= m_energyMax ? m_energyMax : (value <= 0 ? 0 : value);
}
void Character::SetMana(const float value)
{
	m_mana = value >= m_manaMax ? m_manaMax : (value <= 0 ? 0 : value);
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