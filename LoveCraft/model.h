#ifndef MODEL_H__
#define MODEL_H__

#include "mesh.h"
#include "vector2.h"
#include "vector3.h"

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>       
#include <assimp/postprocess.h>

#include <string>

class Model : public Mesh
{
public:
	Model();
	virtual ~Model();

	bool Load(const std::string filePath);

	virtual void SetMeshData(VertexData* vd, int vertexCount);

	virtual void Render(bool wireFrame = false) const;

	void Rotate(float x, float y, float z);
	void Translate(float x, float y, float z);

	void Rotate(Vector3f rot);
	void Translate(Vector3f trans);

	Vector3f GetPosition() const;
	void SetPosition(const Vector3f& pos);
	Vector3f GetRotation() const;
	void SetRotation(const Vector3f& rot);

private:
	Vector3f m_pos;
	Vector3f m_rot;
};

#endif