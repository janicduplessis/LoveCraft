#ifndef CHUNK_MESH_H__
#define CHUNK_MESH_H__

#include "mesh.h"

class ChunkMesh : public Mesh
{
public:
	ChunkMesh();

	virtual ~ChunkMesh();

	virtual bool IsValid() const;

	virtual void SetMeshData( VertexData* vd, int vertexCount );

	virtual void Render() const;

	virtual int Count() const;

private:

};



#endif
