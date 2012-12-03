#ifndef CHUNK_MESH_H__
#define CHUNK_MESH_H__

#include "mesh.h"
#include "texture.h"

class ChunkMesh : public Mesh
{
public:
	ChunkMesh();

	virtual ~ChunkMesh();

	virtual bool IsValid() const;

	virtual void SetMeshData(VertexData* vd, int vertexCount, uint16* indexData = 0, int indexCount = 0);

	virtual void Render(bool wireFrame = false) const;

	virtual int Count() const;

private:
	Texture m_texFloor;
	Texture m_texDirt;
	Texture m_texGrass;
	Texture m_texOctoFace;
	Texture m_texWall;
	Texture m_texCeilling;
};



#endif
