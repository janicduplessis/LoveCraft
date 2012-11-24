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
	glPushMatrix();
	glTranslatef(m_pos.x, m_pos.y, m_pos.z);
	glRotatef(-m_rot.x, 1.f, 0.f, 0.f);
	glRotatef(-m_rot.y, 0.f, 1.f, 0.f);
	glRotatef(-m_rot.z, 0.f, 0.f, 1.f);
	Mesh::Render(wireFrame);
	glPopMatrix();
}

void Model::Rotate( float x, float y, float z )
{
	Rotate(Vector3f(x, y, z));
}

void Model::Rotate( Vector3f rot )
{
	m_rot += rot;
}

void Model::Translate( float x, float y, float z )
{
	Translate(Vector3f(x, y, z));
}

void Model::Translate( Vector3f trans )
{
	m_pos += trans;
}

void Model::SetPosition(const Vector3f& pos) {
	m_pos = pos;
}

Vector3f Model::GetPosition() const
{
	return m_pos;
}

void Model::SetRotation(const Vector3f& rot) {
	m_rot = rot;
}

Vector3f Model::GetRotation() const
{
	return m_rot;
}

