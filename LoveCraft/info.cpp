#include "info.h"

Info::Info()
{
	GenerateBlocInfos();
}

Info::Info( Info const& copy )
{

}

Info::~Info()
{
	for (int i = 0; i < BTYPE_COUNT; ++i)
	{
		delete m_blocInfos[i];
	}
	delete m_info;
}


Info& Info::Get()
{
	// Si info n'est pas instancié
	if (!m_info) {
		// instancie
		m_info = new Info;
	}

	return *m_info;
}

Info* Info::GetPtr()
{
	// Si info n'est pas instancié
	if (!m_info) {
		// instancie
		m_info = new Info;
	}

	return m_info;
}

BlockInfo* Info::GetBlocInfo( BlockType type )
{
	return m_blocInfos[type];
}

void Info::GenerateBlocInfos()
{
	m_blocInfos[0] = new BlockInfo(BTYPE_AIR, "Air");

	m_blocInfos[1] = new BlockInfo(BTYPE_DIRT, "Dirt");

	m_blocInfos[2] = new BlockInfo(BTYPE_GRASS, "Grass");

	m_blocInfos[3] = new BlockInfo(BTYPE_BRICK, "Brick");

	m_blocInfos[4] = new BlockInfo(BTYPE_SAND, "Sand");
}

Info* Info::m_info = 0;