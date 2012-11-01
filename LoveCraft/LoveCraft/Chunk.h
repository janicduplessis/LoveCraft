/*
 *	Chunk.h
 *
 *	Created on: 2012-10-31
 *	Author: Janic Duplessis
 */

#ifndef CHUNK_H_
#define CHUNK_H_

#include "BlocArray3d.h"

/**
 * @brief Chunk, partie de monde
 * 
 * Partie de monde chargee en memoire
 * pour creer un monde infini.
 * Herite de BlocArray3d
 */
class Chunk : public BlocArray3d
{
public:

	/**
	 * Constructeur avec les dimentions des constantes CHUNK_SIZE
	 */
	Chunk();

	/**
	 * Enleve un bloc
	 * @param x		position en x
	 * @param y		position en y
	 * @param z		position en z
	 */
	void RemoveBloc(int x, int y, int z);

	/**
	 * Set un bloc
	 * @param x		position en x
	 * @param y		position en y
	 * @param z		position en z
	 * @param type	type de bloc
	 */
	void SetBloc(int x, int y, int z, BlocType type);


	/**
	 * Get un bloc
	 * @param x		position en x
	 * @param y		position en y
	 * @param z		position en z
	 * @return		BlocType
	 */
	BlocType GetBloc(int x, int y , int z);

	/**
	 * Destructeur
	 */
	~Chunk();

private:

};

#endif // !CHUNK_H_
