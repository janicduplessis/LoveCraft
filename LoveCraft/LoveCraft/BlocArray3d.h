/*
 *	BlocArray3d.h
 *
 *	Created on: 2012-10-31
 *	Author: Janic Duplessis
 */

#ifndef BLOC_ARRAY_3D_H_
#define BLOC_ARRAY_3D_H_

#include "BlocInfo.h"

/**
 * @brief Tableau 3d de BlocTypes
 */ 
class BlocArray3d
{
public:
	/**
	 * Constructeur
	 * @param x	 nombre d'elements en x
	 * @param y	 nombre d'elements en y
	 * @param z	 nombre d'elements en z
	 */
	BlocArray3d(int x, int y, int z);

	/**
	 * Constructeur de copie
	 */
	BlocArray3d(const BlocArray3d &);

	/**
	 * Set le type d'un bloc à la position specifiée
	 * @param x		position en x dans le tableau
	 * @param y		position en y dans le tableau
	 * @param z		position en z dans le tableau
	 * @param type	type a changer
	 */
	void Set(int x, int y, int z, BlocType type);

	/**
	 * Get le type d'un bloc à la position specifiée
	 * @param x		position en x dans le tableau
	 * @param y		position en y dans le tableau
	 * @param z		position en z dans le tableau
	 * @return		type du bloc
	 */
	BlocType Get(int x, int y, int z) const;

	/**
	 * Set tous les blocs au type spécifié
	 * @param type	type a set
	 */
	void Reset(BlocType type);

	/**
	 * Destructeur
	 */
	~BlocArray3d();

private:
	BlocType* m_blocs; // Pointeur
	int m_lenght; // Taille totale
	int m_x; // Taille x
	int m_y; // Taille y
	int m_z; // Taille z
};

#endif // !BLOC_ARRAY_3D_H_
