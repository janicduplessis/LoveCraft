#include "character.h"


Character::Character() : m_name("DeFacto"), m_healthMax(HEALTH_MAX), m_energyMax(ENERGY_MAX), m_manaMax(MANA_MAX),
	m_health(m_healthMax), m_energy(m_energyMax), m_mana(m_manaMax), m_expToNextLevel(10000.f), m_exp(6258.f)
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
void Character::PassiveRegen()
{
	SetHealth(HEALTH_PASSIVE_REGEN);
	SetMana(MANA_PASSIVE_REGEN);
}
void Character::SetHealth(const float value)
{
	float increment = value;
	//vérification que l'ajout ne dépasse pas le maximum
	if (m_health + increment >= m_healthMax)
	{
		m_health = m_healthMax;
		return;
	}
	//Vérification que l'ajout ne dépasse pas 0
	if (m_health + increment <= 0)
	{
		m_health = 0;
		return;
	}
	m_health += increment;
}
void Character::SetEnergy(const float value)
{
	float increment = value;
	//vérification que l'ajout ne dépasse pas le maximum
	if (m_energy + increment >= m_energyMax)
	{
		m_energy = m_energyMax;
		return;
	}
	//Vérification que l'ajout ne dépasse pas 0
	if (m_energy + increment <= 0)
	{
		m_energy = 0;
		return;
	}
	m_energy += increment;
}
void Character::SetMana(const float value)
{
	float increment = value;
	//vérification que l'ajout ne dépasse pas le maximum
	if (m_mana + increment >= m_manaMax)
	{
		m_mana = m_manaMax;
		return;
	}
	//Vérification que l'ajout ne dépasse pas 0
	if (m_mana + increment <= 0)
	{
		m_mana = 0;
		return;
	}
	m_mana += increment;
}
void Character::SetExp(const float value)
{
	float increment = value;
	//vérification que l'ajout ne dépasse pas le maximum
	if (m_exp + increment >= m_expToNextLevel)
	{
		m_exp = m_expToNextLevel;
		return;
	}
	//Vérification que l'ajout ne dépasse pas 0
	if (m_exp + increment <= 0)
	{
		m_exp = 0;
		return;
	}
	m_exp += increment;
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
float Character::Exp() const
{
	return m_exp;
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
float Character::ExpNext() const
{
	return m_expToNextLevel;
}

float Character::HealthPerc() const
{
	return (m_health / m_healthMax * 100);
}
float Character::EnergyPerc() const
{
	return (m_energy / m_energyMax * 100);
}
float Character::ManaPerc() const
{
	return (m_mana / m_manaMax * 100);
}
float Character::ExpPerc() const
{
	return (m_exp / m_expToNextLevel * 100);
}