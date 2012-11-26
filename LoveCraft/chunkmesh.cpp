#include "chunkmesh.h"


ChunkMesh::ChunkMesh()
{
	//Chargement des textures
	m_texFloor.Load(TEXTURE_PATH "checker.bmp");
	m_texOctoFace.Load(TEXTURE_PATH "cu_default.png");
	m_texWall.Load(TEXTURE_PATH "wall-red.bmp");
	m_texCeilling.Load(TEXTURE_PATH "checker-gold.bmp");
	m_texDirt.Load(TEXTURE_PATH "dirt.bmp");
	m_texGrass.Load(TEXTURE_PATH "grass.bmp");
}

ChunkMesh::~ChunkMesh()
{

}

bool ChunkMesh::IsValid() const
{
	return Mesh::IsValid();
}

void ChunkMesh::SetMeshData( VertexData* vd, int vertexCount )
{
	return Mesh::SetMeshData(vd, vertexCount);
}

void ChunkMesh::Render() const
{
	m_texDirt.Bind();
	Mesh::Render();
}

int ChunkMesh::Count() const
{
	return Mesh::Count();
}
