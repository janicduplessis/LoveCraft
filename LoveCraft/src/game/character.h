#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "define.h"
#include <iostream>
#include <string>

/**
* @brief Classe repr�sentant les infos du personnage
*/
class Character
{
public:
	/**
	* Constructeur par d�faut
	*/
	Character();
	/**
	* Constructeur avec param�tres
	* @param name		Le nom du personnage
	* @param health	La vie maximum du personnage
	* @param energy	L'�nergie maximum du joueur
	* @param mana		Le mana maximum du personnage
	*/
	Character(std::string name, float health, float energy, float mana);
	/**
	* Destructeur par d�faut
	*/
	~Character();
	/**
	* Le nom du joueur
	* @return std::string
	*/
	std::string Name() const;
	/**
	* Change la vie du personnage
	* @param value		La valeur voulue - soumise � des validations
	*/
	void SetHealth(const float value);
	/**
	* Change l'�nergie du personnage
	* @param value		La valeur voulue - soumise � des validations
	*/
	void SetEnergy(const float value);
	/**
	* Change la mana du personnage
	* @param value		La valeur voulue - soumise � des validations
	*/
	void SetMana(const float value);

	/**
	* Obtient la vie courante du personnage
	* @return float
	*/
	float Health() const;
	/**
	* Obtient l'�nergie courante du personnage
	* @return float
	*/
	float Energy() const;
	/**
	* Obtient la mana courante du personnage
	* @return float
	*/
	float Mana() const;

	/**
	* Obtient la vie maximum du personnage
	* @return float
	*/
	float HealthMax() const;
	/**
	* Obtient l'�nergie maximum du personnage
	* @return float
	*/
	float EnergyMax() const;
	/**
	* Obtient la mana maximum du personnage
	* @return float
	*/
	float ManaMax() const;

	/**
	* Obtient le pourventage de vie du personnage
	* @return float
	*/
	float HealthPerc() const;
	/**
	* Obtient le pourventage d'�nergie du personnage
	* @return float
	*/
	float EnergyPerc() const;
	/**
	* Obtient le pourventage de mana du personnage
	* @return float
	*/
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