/*
 *	BlocInfo.cpp
 *
 *	Created on: 2012-10-31
 *	Author: Janic Duplessis
 */

#include "BlocInfo.h"
#include <iostream>

BlocInfo::BlocInfo(BlocType type, const std::string& nom) : m_type(type), m_nom(nom)
{
	m_durabilite = 0;
}

BlocType BlocInfo::GetType() const
{
	return m_type;
}

void BlocInfo::SetDurabilite(int durabilite)
{
	m_durabilite = durabilite;
}

int BlocInfo::GetDurabilite() const
{
	return m_durabilite;
}

void BlocInfo::Afficher() const
{
	std::cout << "Nom" << " : " << m_nom << std::endl;
	std::cout << "Type" << " : " << m_type << std::endl;
	std::cout << "Durabilite" << " : " << m_durabilite << std::endl;
}

BlocInfo::~BlocInfo() {}
