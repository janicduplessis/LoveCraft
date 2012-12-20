#ifndef CHUNK_H__
#define CHUNK_H__
#include <iostream>
#include "array3d.h"
#include "define.h"
#include "chunkmesh.h"
#include "info.h"
#include "shader.h"

class Chunk
{
	enum MeshFace
	{
		FACE_TOP,
		FACE_BOTTOM,
		FACE_FRONT,
		FACE_BACK,
		FACE_LEFT,
		FACE_RIGHT
	};

public:
	Chunk(Shader* shader = 0);
	~Chunk();

	void RemoveBloc(uint32 x, uint32 y, uint32 z);
	void SetBloc(uint32 x, uint32 y, uint32 z, BlockType type);
	BlockType GetBloc(uint32 x, uint32 y, uint32 z);

	void Update();
	void Render() const;
	bool IsDirty() const;

	Vector2i GetPosition() const;
	Vector2f GetRealPosition() const;
	void SetPosition(Vector2i pos);
	
private:
	void CreateOptimizedTopBottomFace(MeshFace face, ChunkMesh::VertexData* vd, int& vertexCount, ChunkMesh::TextureData* td, int x, int y, int z, BlockType bt, Array3d<bool>& facesOptimized);
	void CreateOptimizedFrontBackFace(MeshFace face, ChunkMesh::VertexData* vd, int& vertexCount, ChunkMesh::TextureData* td, int x, int y, int z, BlockType bt, Array3d<bool>& facesOptimized);
	void CreateOptimizedLeftRightFace(MeshFace face, ChunkMesh::VertexData* vd, int& vertexCount, ChunkMesh::TextureData* td, int x, int y, int z, BlockType bt, Array3d<bool>& facesOptimized);

private:
	bool m_isDirty;
	Vector2i m_pos;
	Array3d<BlockType> m_blocks;
	ChunkMesh m_chunkMesh;
};

#endif // CHUNK_H__