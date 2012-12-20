#ifndef CHUNKMESH_H__
#define CHUNKMESH_H__

#include "mesh.h"
#include "texture.h"
#include "shader.h"

class ChunkMesh : public Mesh
{
public:
	// Structure représentant toutes les informations de chacuns des vertex
	// S'assurer que le size de cette struct reste un multiple de 32
	// octet pour un maximum de performance
	// Au besoin, ajouter du padding
	struct VertexData
	{
		float x, y, z;
		float r, g, b;

		VertexData() {}
		VertexData(float _x, float _y, float _z, float _r, float _g, float _b) : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b) {}
	};

	struct TextureData
	{
		float u, v;
		float id;

		TextureData() {}
		TextureData(float _u, float _v, float _id) : u(_u), v(_v), id(_id) {}
	};

public:
	ChunkMesh(Shader* shader = 0);

	virtual ~ChunkMesh();

	virtual bool IsValid() const;

	virtual void SetMeshData(VertexData* vd, int vertexCount, TextureData* td);

	virtual void Render(bool wireFrame = false) const;

	virtual int Count() const;

private:
	Shader* m_shader;
	GLuint m_textureVboId;
};

#endif
