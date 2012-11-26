#ifndef CHUNK_H__
#define CHUNK_H__
#include <iostream>
#include "array3d.h"
#include "define.h"
#include "chunkmesh.h"

class Chunk
{
public:
	Chunk();
	~Chunk();

	void RemoveBloc(int x, int y, int z);
	void SetBloc(int x, int y, int z, BLOCK_TYPE type);
	BLOCK_TYPE GetBloc(int x, int y, int z);

	void Update();
	void Render() const;
	bool IsDirty() const;
	void AddBlockToMesh(ChunkMesh::VertexData* vd, int& count, BLOCK_TYPE bt, int x, int y, int z);

private:
	bool m_isDirty;
	Array3d<BLOCK_TYPE> m_blocks;
	ChunkMesh m_chunkMesh;
};

#endif // CHUNK_H__
