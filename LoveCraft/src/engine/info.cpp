#include "info.h"
#include "engine/gl/ui/label.h"
Info::Info()
{
	GenerateBlocInfos();
	if (!m_sound.LoadSounds())
		std::cout << "Une erreur est survenue lors du chargement des sons en memoire" << std::endl;

	m_fontColors = new Texture*[Label::TEXTCOLOR_LAST];
	for (unsigned short i = 0; i < Label::TEXTCOLOR_LAST; i++)
		m_fontColors[i] = 0;
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
	// Si info n'est pas instanciť
	if (!m_info) {
		// instancie
		m_info = new Info;
	}

	return *m_info;
}

Info* Info::GetPtr()
{
	// Si info n'est pas instanciť
	if (!m_info) {
		// instancie
		m_info = new Info;
	}

	return m_info;
}

Son& Info::Sound()
{
	return m_sound;
}

Option& Info::Options()
{
	return m_options;
}

Nwork& Info::Network()
{
	return m_network;
}

BlockInfo* Info::GetBlocInfo( BlockType type )
{
	return m_blocInfos[type];
}

BlockType Info::GetBlocFromWorld(Vector3f pos, const Vector3f& offset) const
{
	// Ajoute le offset
	pos += offset;
	// Replace la position par rapport au premier cube
	pos.x += VIEW_DISTANCE + 0.5f;
	pos.z += VIEW_DISTANCE + 0.5f;
	// Enleve la partie decimale
	Vector3i iPos(pos.x, pos.y, pos.z);
	// Calcul dans quel chunk la position est
	int chunkX, chunkZ;
	chunkX = iPos.x / CHUNK_SIZE_X;
	chunkZ = iPos.z / CHUNK_SIZE_Z;
	Chunk& c = GetChunkArray()->Get(chunkX, chunkZ);
	// Calcul dans quel bloc la position est
	int blocX, blocY, blocZ;
	blocX = iPos.x - chunkX * CHUNK_SIZE_X;
	blocY = iPos.y;
	blocZ = iPos.z - chunkZ * CHUNK_SIZE_Z;

	return c.GetBloc(blocX, blocY, blocZ);
}

void Info::GenerateBlocInfos()
{
	m_blocInfos[BTYPE_AIR] = new BlockInfo(BTYPE_AIR, "Air");

	m_blocInfos[BTYPE_DIRT] = new BlockInfo(BTYPE_DIRT, "Dirt");

	m_blocInfos[BTYPE_GRASS] = new BlockInfo(BTYPE_GRASS, "Grass");

	m_blocInfos[BTYPE_BRICK] = new BlockInfo(BTYPE_BRICK, "Brick");

	m_blocInfos[BTYPE_SAND] = new BlockInfo(BTYPE_SAND, "Sand");

	m_blocInfos[BTYPE_ROCK] = new BlockInfo(BTYPE_ROCK, "Rock");

	m_blocInfos[BTYPE_SNOW] = new BlockInfo(BTYPE_SNOW, "Snow");
}

void Info::SetChunkArray( Array2d<Chunk>* arrayPtr )
{
	m_chunks = arrayPtr;
}

Array2d<Chunk>* Info::GetChunkArray() const
{
	return m_chunks;
}


Info* Info::m_info = 0;