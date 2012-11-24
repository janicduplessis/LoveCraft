#include "model.h"
#include <iostream>


Model::Model() {}

Model::~Model() {}

bool Model::Load( const std::string filePath )
{

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile( filePath, aiProcessPreset_TargetRealtime_Quality);

	// If the import failed
	if( !scene)
	{
		std::cout << "failed to load model : " << filePath << std::endl;
		return false;
	}

	// Converti le model importé en VBO
	for (uint32 i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		VertexData* vertices = new VertexData[mesh->mNumVertices];

		// Converti en struct VertexData
		for (uint32 j = 0; j < mesh->mNumVertices; j++)
		{
			VertexData& v = vertices[j];

			v.x = mesh->mVertices[j].x;
			v.y = mesh->mVertices[j].y;
			v.z = mesh->mVertices[j].z;

			if (mesh->HasVertexColors(j)) {
				v.r = mesh->mColors[j]->r;
				v.b = mesh->mColors[j]->b;
				v.g = mesh->mColors[j]->g;
			}

			if (mesh->HasTextureCoords(j))
			{
				v.u = mesh->mTextureCoords[j]->x;
				v.v = mesh->mTextureCoords[j]->y;
			}

			if (mesh->HasNormals()) {
				v.n1 = mesh->mNormals[j].x;
				v.n2 = mesh->mNormals[j].x;
				v.n3 = mesh->mNormals[j].x;
			}

		}

		SetMeshData(vertices, mesh->mNumVertices);

		if (vertices)
			delete [] vertices;
	}

	return true;

}

void Model::SetMeshData( VertexData* vd, int vertexCount )
{

	Mesh::SetMeshData(vd, vertexCount);
}

void Model::Render(bool wireFrame) const
{
	Mesh::Render(wireFrame);
}

