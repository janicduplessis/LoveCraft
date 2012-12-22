#ifndef MODEL_H__
#define MODEL_H__

#include "mesh.h"
#include "vector2.h"
#include "vector3.h"

#include <string>

class Model : public Mesh
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
		float u, v;
		float n1, n2, n3;

		VertexData() {}
		VertexData(float _x, float _y, float _z, float _r, float _g, float _b, float _u, float _v) : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), u(_u), v(_v) {}
	};

public:
	Model();
	virtual ~Model();

	bool Load(const std::string filePath);

	virtual void Render(bool wireFrame = false) const;
	virtual void SetMeshData(VertexData* vd, int vertexCount, uint16* indexData = 0, int indexCount = 0);

	void Rotate(float x, float y, float z);
	void Translate(float x, float y, float z);
	void Scale(float x, float y, float z);

	void Rotate(const Vector3f& rot);
	void Translate(const Vector3f& trans);
	void Scale(const Vector3f& scale);

	Vector3f GetPosition() const;
	void SetPosition(const Vector3f& pos);
	Vector3f GetRotation() const;
	void SetRotation(const Vector3f& rot);

private:
	Vector3f m_pos;
	Vector3f m_rot;
	Vector3f m_scale;
};

#endif