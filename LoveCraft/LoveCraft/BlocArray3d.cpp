/*
 *	BlocArray3d.cpp
 *
 *	Created on: 2012-10-31
 *	Author: Janic Duplessis
 */

#include "BlocArray3d.h"

BlocArray3d::BlocArray3d(int x, int y, int z) : m_lenght(x * y * z), m_x(x), m_y(y), m_z(z)
{
	// Allouer espace en memoire
	m_blocs = new BlocType[m_lenght];

	// Initialiser avec des BTYPE_AIR
	Reset(BTYPE_AIR);
}

void BlocArray3d::Set(int x, int y, int z, BlocType type)
{
	m_blocs[x + (z * m_x) + (y * m_z * m_x)] = type;
}

BlocType BlocArray3d::Get(int x, int y, int z) const
{
	return m_blocs[x + (z * m_x) + (y * m_z * m_x)];
}

void BlocArray3d::Reset(BlocType type)
{
	// Set tous les blocs au type donne
	for (int i = 0; i < m_lenght; ++i)
	{
		m_blocs[i] = type;
	}
}

BlocArray3d::~BlocArray3d()
{
	delete[] m_blocs;
}