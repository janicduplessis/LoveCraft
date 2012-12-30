#include "blockinfo.h"
#include <iostream>

BlockInfo::BlockInfo(BlockType type, const std::string& nom) : m_type(type), m_nom(nom), m_durabilite(1)
{
}

BlockInfo::~BlockInfo()
{
}

BlockType BlockInfo::GetType() const
{
	return m_type;
}
Son::Foots BlockInfo::Getsound() const
{
	switch (m_type)
	{
	case BLOCK_TYPE::BTYPE_BRICK:
		return Son::FOOT_CONCRETE;
	case BLOCK_TYPE::BTYPE_GRASS:
		return Son::FOOT_GRASS;
	case BLOCK_TYPE::BTYPE_DIRT:
		return Son::FOOT_MUD;
	case BLOCK_TYPE::BTYPE_SAND:
		return Son::FOOT_DIRT;
	default:
		return Son::FOOT_AIR;
		break;
	}
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

void BlockInfo::SetTextureIndex( TextureArray::TextureIndex coords )
{
	m_textureIndex = coords;
}

TextureArray::TextureIndex BlockInfo::GetTextureIndex() const
{
	return m_textureIndex;
}


