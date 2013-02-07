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
	Chunk(const Vector2i& arrayPos, const Vector2f& worldPos);
	~Chunk();

	void RemoveBloc(uint32 x, uint32 y, uint32 z);
	void SetBloc(int x, int y, int z, BlockType type);
	BlockType GetBloc(int x, int y, int z);

	void Update();
	void Render() const;
	bool IsDirty() const;
	bool IsReady() const;
	void SetIsReady(bool val);

	Vector2f GetWorldPosition() const;
	Vector2i GetArrayPosition() const;
	void SetWorldPosition(Vector2f pos);

	void SetSurroundings(Vector2i arrayPos);
	void GenerateTrees();

	
private:
	void CreateOptimizedTopBottomFace(MeshFace face, ChunkMesh::VertexData* vd, int& vertexCount, int x, int y, int z, BlockType bt, Array3d<bool>& facesOptimized);
	void CreateOptimizedFrontBackFace(MeshFace face, ChunkMesh::VertexData* vd, int& vertexCount, int x, int y, int z, BlockType bt, Array3d<bool>& facesOptimized);
	void CreateOptimizedLeftRightFace(MeshFace face, ChunkMesh::VertexData* vd, int& vertexCount, int x, int y, int z, BlockType bt, Array3d<bool>& facesOptimized);
	bool NoCloseTree(Vector3f pos);
	bool CheckBoolArray(const Array3d<bool>& array, int x, int y, int z) const;
private:
	bool m_isDirty;
	bool m_isReady;
	Vector2i m_chunkArrayPos;
	Vector2f m_worldPos;
	Array3d<BlockType> m_blocks;
	ChunkMesh m_chunkMesh;

	Chunk* m_left;
	Chunk* m_right;
	Chunk* m_front;
	Chunk* m_back;
};

#endif // CHUNK_H__