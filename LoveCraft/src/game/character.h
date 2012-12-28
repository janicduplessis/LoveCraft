#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "define.h"
#include <iostream>
#include <string>

/**
* @brief Classe représentant les infos du personnage
*/
class Character
{
public:
	Character();
	Character(std::string name, float health, float energy, float mana);
	~Character();
	std::string Name() const;
	void SetHealth(const float value);
	void SetEnergy(const float value);
	void SetMana(const float value);

	float Health() const;
	float Energy() const;
	float Mana() const;

	float HealthMax() const;
	float EnergyMax() const;
	float ManaMax() const;

	float HealthPerc() const;
	float EnergyPerc() const;
	float ManaPerc() const;
private:
	std::string m_name;

	float m_healthMax;
	float m_energyMax;
	float m_manaMax;

	float m_health;
	float m_energy;
	float m_mana;
};

#endif