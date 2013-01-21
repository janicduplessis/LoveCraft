#ifndef CHUNK_H__
#define CHUNK_H__

#include "define.h"

#include "info.h"
#include "gl/chunkmesh.h"
#include "gl/shaders/shader.h"
#include <iostream>

#include <util/array3d.h>

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
	Chunk(Vector2i pos, Shader* shader = 0);
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

	void SetSurroundings();
	void GenerateTrees();

	
private:
	void CreateOptimizedTopBottomFace(MeshFace face, ChunkMesh::VertexData* vd, int& vertexCount, ChunkMesh::TextureData* td, int x, int y, int z, BlockType bt, Array3d<bool>& facesOptimized);
	void CreateOptimizedFrontBackFace(MeshFace face, ChunkMesh::VertexData* vd, int& vertexCount, ChunkMesh::TextureData* td, int x, int y, int z, BlockType bt, Array3d<bool>& facesOptimized);
	void CreateOptimizedLeftRightFace(MeshFace face, ChunkMesh::VertexData* vd, int& vertexCount, ChunkMesh::TextureData* td, int x, int y, int z, BlockType bt, Array3d<bool>& facesOptimized);
	bool NoCloseTree(Vector3f pos);
private:
	bool m_isDirty;
	Vector2i m_pos;
	Array3d<BlockType> m_blocks;
	ChunkMesh m_chunkMesh;

	Chunk* m_left;
	Chunk* m_right;
	Chunk* m_front;
	Chunk* m_back;
};

#endif // CHUNK_H__