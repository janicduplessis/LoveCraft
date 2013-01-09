#include "character.h"
#include <sstream>
#include <string>

Character::Character() : m_name("DeFacto"), m_healthMax(HEALTH_MAX), m_energyMax(ENERGY_MAX), m_manaMax(MANA_MAX),
	m_health(m_healthMax), m_energy(m_energyMax), m_mana(m_manaMax), m_exp(0), m_globalCooldown(0), m_level(1)
{
	m_expTable = new uint16[CHARACTER_MAX_LEVEL];
	m_expTable[0] = 50;
	m_expTable[1] = 110;
	m_expTable[2] = 190;
	m_expTable[3] = 260;
	m_expTable[4] = 350;
	m_expTable[5] = 470;
	m_expTable[6] = 600;
	m_expTable[7] = 780;
	m_expTable[8] = 970;
	m_expTable[9] = 0;
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
uint16 Character::Level() const
{
	return m_level;
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
	//Vérification que l'ajout ne dépasse pas 0
	if (m_exp + increment <= 0)
	{
		m_exp = 0;
		return;
	}
	m_exp += increment;
	CheckLevelUp();
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
	return m_expTable[m_level-1];
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
	return (m_exp / ExpNext() * 100);
}

float Character::GlobalCooldown() const
{
	return m_globalCooldown;
}

void Character::ReduceGlobalCooldown( float time )
{
	if (GlobalCooldown() == 0)
		return;
	float nextGCDTime = m_globalCooldown - time;
	if (nextGCDTime > 0)
	{
		m_globalCooldown = nextGCDTime;
	}
	else m_globalCooldown = 0;
}

void Character::ResetGlobalCooldown()
{
	m_globalCooldown = GLOBAL_COOLDOWN;
}

//Private

bool Character::CheckLevelUp()
{
	if (m_level < CHARACTER_MAX_LEVEL)
	{
		if (m_exp >= m_expTable[m_level-1])
		{
			m_exp -= m_expTable[m_level-1];
			m_level++;
			ApplyBonus();
			Info::Get().Sound().PlaySnd(Son::SON_LEVELUP, Son::CHANNEL_INTERFACE, true);
			std::ostringstream ss;
			ss << "Felicitations! Vous etes passe(e) au niveau " << m_level << "!";
			Info::Get().NextPrint(ss.str());
			return true;
		}
	}
	return false;
}
void Character::ApplyBonus()
{
	m_healthMax += CHARACTER_HEALTH_INCREASE;
	m_manaMax += CHARACTER_MANA_INCREASE;
	m_energyMax += CHARACTER_ENERGY_INCREASE;
}