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
		Vector3f tex;		// Texture uv coords et index
		Vector3f normal;	// Normale
		Vector3f tangent;	// Tangente pour calculer les normal map

		VertexData() {}

		VertexData(float _x, float _y, float _z, float _u, float _v, float _id) :
			pos(_x, _y, _z), tex(_u, _v, _id), normal(0), tangent(0) {}

		VertexData(float _x, float _y, float _z, float _u, float _v, float _id, float n1, float n2, float n3) :
			pos(_x, _y, _z), tex(_u, _v, _id), normal(n1, n2, n3), tangent(0) {}

		VertexData(float _x, float _y, float _z, float _u, float _v, float _id, float n1,
			float n2, float n3, float t1, float t2, float t3) :
			pos(_x, _y, _z), tex(_u, _v, _id), normal(n1, n2, n3), tangent(t1, t2, t3) {}
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
