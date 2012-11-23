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
	~Model();

	bool Load(const std::string filePath);

private:

};

#endif