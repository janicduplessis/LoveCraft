#include "model.h"


Model::Model() : Mesh()
{

}

Model::~Model()
{

}

bool Model::Load( const std::string filePath )
{

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile( filePath, aiProcessPreset_TargetRealtime_Quality);

	// If the import failed
	if( !scene)
	{
		return false;
	}

	// Converti le model importé en VBO
	for (uint32 i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		VertexData* vertices = new VertexData[mesh->mNumVertices];

		// Converti en struct VertexData
		for (int j = 0; j < mesh->mNumVertices; j++)
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
		}

		Mesh::SetMeshData(vertices, mesh->mNumVertices);

		if (vertices)
			delete [] vertices;
	}

	return true;

}