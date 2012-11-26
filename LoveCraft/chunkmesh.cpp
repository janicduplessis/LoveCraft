#include "chunkmesh.h"


ChunkMesh::ChunkMesh()
{

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
	Mesh::Render();
}

int ChunkMesh::Count() const
{
	return Mesh::Count();
}
