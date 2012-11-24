#ifndef MODEL_H__
#define MODEL_H__

#include "mesh.h"

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

private:

};

#endif