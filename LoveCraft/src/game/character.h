﻿#ifndef CHARACTER_H_
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
	/**
	* Constructeur par défaut
	*/
	Character();
	/**
	* Constructeur avec paramètres
	* @param name		Le nom du personnage
	* @param health	La vie maximum du personnage
	* @param energy	L'énergie maximum du joueur
	* @param mana		Le mana maximum du personnage
	*/
	Character(std::string name, float health, float energy, float mana);
	/**
	* Destructeur par défaut
	*/
	~Character();
	/**
	* Le nom du joueur
	* @return std::string
	*/
	std::string Name() const;
	/**
	* Méthode à appeler à chaque fin de boucle pour la régénération
	* de la vie et du mana
	*/
	void PassiveRegen();
	/**
	* Change la vie du personnage. Une valeur négative diminue, positive augmente.
	* @param value		La valeur voulue - soumise à des validations
	*/
	void SetHealth(const float value);
	/**
	* Change l'énergie du personnage. Une valeur négative diminue, positive augmente.
	* @param value		La valeur voulue - soumise à des validations
	*/
	void SetEnergy(const float value);
	/**
	* Change la mana du personnage. Une valeur négative diminue, positive augmente.
	* @param value		La valeur voulue - soumise à des validations
	*/
	void SetMana(const float value);

	/**
	* Obtient la vie courante du personnage
	* @return float
	*/
	float Health() const;
	/**
	* Obtient l'énergie courante du personnage
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
	* Obtient l'énergie maximum du personnage
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
	* Obtient le pourventage d'énergie du personnage
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