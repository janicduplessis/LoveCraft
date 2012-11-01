/*
 *	BlocInfo.h
 *
 *	Created on: 2012-10-31
 *	Author: Janic Duplessis
 */

#ifndef BLOC_INFO_H_
#define BLOC_INFO_H_

#include "define.h"
#include <string>

/**
 * @brief Information sur les blocs
 * 
 * Sert a instancier les blocs
 */
class BlocInfo
{
public:
	/**
	 * Constructeur
	 * @param type	type de bloc
	 * @param nom	nom du bloc
	 */
	BlocInfo(BlocType type, const std::string& nom);

	/**
	 * Get le type du bloc
	 */
	BlocType GetType() const;

	/**
	 * Set la durabilite
	 */
	void SetDurabilite(int durabilite);

	/**
	 * Get la durabilite
	 */
	int GetDurabilite() const;

	/**
	 * Affiche les parametres du bloc en console
	 */
	void Afficher() const;

	/**
	 * Destructeur
	 */
	~BlocInfo();

private:
	BlocType m_type; // Type du bloc
	std::string m_nom; // Nom du bloc
	int m_durabilite; // Durabilite du bloc
};

#endif