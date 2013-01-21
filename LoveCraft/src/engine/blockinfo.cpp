#include "blockinfo.h"
#include <iostream>

BlockInfo::BlockInfo(BlockType type, const std::string& nom, bool isSolid) : m_type(type), m_nom(nom), m_durabilite(1), m_isSolid(isSolid)
{
}

BlockInfo::~BlockInfo()
{
}

BlockType BlockInfo::GetType() const
{
	return m_type;
}

void BlockInfo::SetDurabilite(int durabilite)
{
	m_durabilite = durabilite;
}

int BlockInfo::GetDurabilite() const
{
	return m_durabilite;
}

void BlockInfo::Afficher() const
{
	std::cout << "Type: " << m_type << std::endl;
	std::cout << "Nom: " << m_nom << std::endl;
	std::cout << "Durabilite: " << m_durabilite << std::endl;
}

void BlockInfo::SetTextureIndex( int index, TextureArray::TextureIndex coords )
{
	m_textureIndex[index] = coords;
}

TextureArray::TextureIndex BlockInfo::GetTextureIndex(int index) const
{
	return m_textureIndex[index];
}

bool BlockInfo::IsSolid() const
{
	return m_isSolid;
}


