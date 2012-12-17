#ifndef CHUNK_H__
#define CHUNK_H__
#include <iostream>
#include "array3d.h"
#include "define.h"
#include "chunkmesh.h"
#include "info.h"

class Chunk
{
public:
	Chunk();
	~Chunk();

	void RemoveBloc(uint32 x, uint32 y, uint32 z);
	void SetBloc(uint32 x, uint32 y, uint32 z, BlockType type);
	BlockType GetBloc(uint32 x, uint32 y, uint32 z);

	void Update();
	void Render() const;
	bool IsDirty() const;
	void AddBlockToMesh(ChunkMesh::VertexData* vd, int& count, uint16* id, int& indexCount, BlockType bt, int x, int y, int z);

	Vector2i GetPosition() const;
	void SetPosition(Vector2i pos);

private:
	bool m_isDirty;
	Vector2i m_pos;
	Array3d<BlockType> m_blocks;
	ChunkMesh m_chunkMesh;
};

#endif // CHUNK_H__
