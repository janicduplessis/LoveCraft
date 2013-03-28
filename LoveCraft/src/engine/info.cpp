﻿#include "info.h"
#include "engine/gl/ui/controls/localizable/textual/label.h"
#include "interfaceinfos.h"

Info::Info() : m_lineToPrint(""), m_console(0), m_lstatus(0), m_offsetMap(0)
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
	delete [] m_blocInfos;
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

Option& Info::Options()
{
	return m_options;
}

Nwork& Info::Network()
{
	return m_network;
}

Controls& Info::Ctrls()
{
	return m_controls;
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
	pos.x += 0.5f - m_offsetMap.x * CHUNK_SIZE_X;
	pos.z += 0.5f - m_offsetMap.y * CHUNK_SIZE_Z;
	// Enleve la partie decimale
	Vector3i iPos(pos.x, pos.y, pos.z);
	// Calcul dans quel chunk la position est
	int chunkX, chunkZ;
	chunkX = iPos.x / CHUNK_SIZE_X;
	chunkZ = iPos.z / CHUNK_SIZE_Z;
	Chunk* c = GetChunkArray()->Get(chunkX, chunkZ);
	// Calcul dans quel bloc la position est
	int blocX, blocY, blocZ;
	blocX = iPos.x - chunkX * CHUNK_SIZE_X;
	blocY = iPos.y;
	blocZ = iPos.z - chunkZ * CHUNK_SIZE_Z;

	return c->GetBloc(blocX, blocY, blocZ);
}

void Info::GenerateBlocInfos()
{
	m_blocInfos = new BlockInfo*[BTYPE_COUNT];

	m_blocInfos[BTYPE_AIR] = new BlockInfo(BTYPE_AIR, "Air", false);

	m_blocInfos[BTYPE_DIRT] = new BlockInfo(BTYPE_DIRT, "Dirt", true);

	m_blocInfos[BTYPE_GRASS] = new BlockInfo(BTYPE_GRASS, "Grass", true);

	m_blocInfos[BTYPE_BRICK] = new BlockInfo(BTYPE_BRICK, "Brick", true);

	m_blocInfos[BTYPE_SAND] = new BlockInfo(BTYPE_SAND, "Sand", true);

	m_blocInfos[BTYPE_ROCK] = new BlockInfo(BTYPE_ROCK, "Rock", true);

	m_blocInfos[BTYPE_SNOW] = new BlockInfo(BTYPE_SNOW, "Snow", true);

	m_blocInfos[BTYPE_SWAMP] = new BlockInfo(BTYPE_SWAMP, "Swamp", false);

	m_blocInfos[BTYPE_TREELEAF] = new BlockInfo(BTYPE_TREELEAF, "Tree leaf", false);

	m_blocInfos[BTYPE_TREETRUNK] = new BlockInfo(BTYPE_TREETRUNK, "Tree trunk", true);
}

void Info::SetChunkArray( Array2d<Chunk*>* arrayPtr )
{
	m_chunks = arrayPtr;
}

Array2d<Chunk*>* Info::GetChunkArray() const
{
	return m_chunks;
}

Shader* Info::GetCubeShader() 
{
	return m_shader;
}

void Info::SetCubeShader(Shader* shader)
{
	m_shader = shader;
}

Camera* Info::GetCamera() const
{
	return m_cam;
}

void Info::SetCamera(Camera* cam)
{
	m_cam = cam;
}

Info* Info::m_info = 0;

void Info::NextPrint(const std::string& text)
{
	m_lineToPrint = text;
}
std::string Info::LineToPrint() const
{
	return m_lineToPrint;
}

Vector2i Info::GetMouse()
{
	return m_mouse;
}

void Info::SetMouse( Vector2i pos )
{
	m_mouse = pos;
}

ListBox* Info::Console()
{
	return m_console;
}

void Info::SetConsole( ListBox* console )
{
	m_console = console;
}

void Info::StatusOn(LoadedStatus status)
{
	m_lstatus = m_lstatus | status;
}
bool Info::GetStatus(LoadedStatus status) const
{
	return (m_lstatus & status == 1);
}

void Info::SetOffsetMap( const Vector2i& offset )
{
	m_offsetMap = offset;
}

Vector2i Info::GetOffsetMap() const
{
	return m_offsetMap;
}

Dice& Info::Dic()
{
	return m_dice;
}