#ifndef CHUNKMESH_H__
#define CHUNKMESH_H__

#include "mesh.h"
#include "texture.h"
#include "util/vector3.h"
#include "util/vector2.h"

class ChunkMesh : public Mesh
{
public:
	// Structure représentant toutes les informations de chacuns des vertex
	// S'assurer que le size de cette struct reste un multiple de 32
	// octet pour un maximum de performance
	// Au besoin, ajouter du padding
	struct VertexData
	{
		Vector3f pos;		// Vertex position
		float u, v, id;		// Texture uv coords et index
		Vector3f normal;	// Normale

		VertexData() {}
		VertexData(float _x, float _y, float _z, float _u, float _v, float _id) :
			pos(_x, _y, _z), normal(0), u(_u), v(_v), id(_id) {}
		VertexData(float _x, float _y, float _z, float _u, float _v, float _id, float n1, float n2, float n3) :
			pos(_x, _y, _z), u(_u), v(_v), id(_id), normal(n1, n2, n3) {}
	};

public:
	ChunkMesh();

	virtual ~ChunkMesh();

	virtual bool IsValid() const;

	virtual void SetMeshData(VertexData* vd, int vertexCount);

	virtual void Render(bool wireFrame = false) const;

	virtual int Count() const;

private:

};

#endif
