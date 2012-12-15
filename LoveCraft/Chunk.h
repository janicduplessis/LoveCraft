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

	void RemoveBloc(int x, int y, int z);
	void SetBloc(int x, int y, int z, BlockType type);
	BlockType GetBloc(int x, int y, int z);

	void Update();
	void Render() const;
	bool IsDirty() const;
	void AddBlockToMesh(ChunkMesh::VertexData* vd, int& count, BlockType bt, int x, int y, int z);

	Vector2i GetPosition() const;
	void SetPosition(Vector2i pos);

private:
	bool m_isDirty;
	Vector2i m_pos;
	Array3d<BlockType> m_blocks;
	ChunkMesh m_chunkMesh;
};

#endif // CHUNK_H__
