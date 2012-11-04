/*
 *	BlocArray3d.cpp
 *
 *	Created on: 2012-10-31
 *	Author: Janic Duplessis
 */

#include "BlocArray3d.h"
#include <iostream>

BlocArray3d::BlocArray3d(int x, int y, int z) : m_lenght(x * y * z), m_x(x), m_y(y), m_z(z)
{
	// Allouer espace en memoire
	m_blocs = new BlocType[m_lenght];

	// Initialiser avec des BTYPE_AIR
	Reset(BTYPE_AIR);
}

BlocArray3d::BlocArray3d(const BlocArray3d& blockArray) : 
	m_lenght(blockArray.m_lenght), m_x(blockArray.m_x), m_y(blockArray.m_y), m_z(blockArray.m_z)
{
	m_blocs = new BlocType[m_lenght];
	for (int i = 0; i < m_lenght; i++)
	{
		m_blocs[i] = blockArray.m_blocs[i];
	}
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
	for (int i = 0; i < m_lenght ; ++i)
	{
		m_blocs[i] = type;
	}
}

BlocArray3d::~BlocArray3d()
{
	delete[] m_blocs;
}