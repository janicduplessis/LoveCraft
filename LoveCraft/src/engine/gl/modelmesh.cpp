#include "modelmesh.h"
#include <cassert>


ModelMesh::MeshEntry::MeshEntry()
{
	VB = INVALID_OGL_VALUE;
	IB = INVALID_OGL_VALUE;
	NumIndices  = 0;
	MaterialIndex = INVALID_MATERIAL;
}

ModelMesh::MeshEntry::~MeshEntry()
{
	if (VB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &VB);
	}

	if (IB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &IB);
	}
}

bool ModelMesh::MeshEntry::Init( const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices )
{
	NumIndices = Indices.size();

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);

	return true;
}


ModelMesh::ModelMesh() : m_scale(1,1,1), m_translation(0,0,0), m_rot(0,0,0), m_pos(0,0,0) {}

ModelMesh::~ModelMesh()
{
	Clear();
}

bool ModelMesh::LoadMesh( const std::string& Filename )
{
	// Delete l'ancien mesh
	Clear();

	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (pScene) {
		Ret = InitFromScene(pScene, Filename);
	}
	else {
		std::cout << "Error parsing " << Filename.c_str() << " : " << Importer.GetErrorString() << std::endl;
	}

	return Ret;
}

bool ModelMesh::InitFromScene( const aiScene* pScene, const std::string& Filename )
{
	m_entries.resize(pScene->mNumMeshes);
	m_textures.resize(pScene->mNumMaterials);

	// Initialise tout les mesh du modele
	for (unsigned int i = 0 ; i < m_entries.size() ; i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	return InitMaterials(pScene, Filename);
}

void ModelMesh::InitMesh( unsigned int Index, const aiMesh* paiMesh )
{
	m_entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
		const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
		const aiVector3D* pTangent	= &(paiMesh->mTangents[i]);

		Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
			Vector3f(pTexCoord->x, pTexCoord->y, 0),
			Vector3f(pNormal->x, pNormal->y, pNormal->z),
			Vector3f(pTangent->x, pTangent->y, pTangent->z));

		Vertices.push_back(v);
	}

	for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	m_entries[Index].Init(Vertices, Indices);
}

bool ModelMesh::InitMaterials( const aiScene* pScene, const std::string& Filename )
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = Filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "/" + Path.data;
				m_textures[i] = new Texture;
				if (!m_textures[i]->Load(FullPath.c_str())) {
					std::cout << "Error loading texture : " << FullPath << std::endl;
					delete m_textures[i];
					m_textures[i] = 0;
					Ret = false;
				}
			}
		}

		// Load a white texture in case the model does not include its own texture
		if (!m_textures[i]) {
			m_textures[i] = new Texture;
			Ret = m_textures[i]->Load(TEXTURE_PATH "white.png");
		}
	}

	return Ret;
}

void ModelMesh::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	for (unsigned int i = 0 ; i < m_entries.size() ; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_entries[i].VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)24);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)36);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_entries[i].IB);

		const unsigned int MaterialIndex = m_entries[i].MaterialIndex;

		if (MaterialIndex < m_textures.size() && m_textures[MaterialIndex]) {
			m_textures[MaterialIndex]->Bind(GL_TEXTURE0);
		}

		glDrawElements(GL_TRIANGLES, m_entries[i].NumIndices, GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void ModelMesh::Clear()
{
	for (unsigned int i = 0 ; i < m_textures.size() ; i++) {
		delete m_textures[i];
	}
}

void ModelMesh::Translate( const Vector3f& trans )
{
	m_translation += trans;
}

void ModelMesh::Scale( const Vector3f& scale )
{
	m_scale.x *= scale.x;
	m_scale.y *= scale.y;
	m_scale.z *= scale.z;
}

void ModelMesh::SetPosition(const Vector3f& pos) {
	m_pos = pos;
}

void ModelMesh::SetRotation(const Vector3f& rot) {
	m_rot = rot;
}

Matrix4f ModelMesh::GetWorldMatrix()
{
	m_worldPos = Matrix4f::IDENTITY;
	m_worldPos.ApplyScale(m_scale.x, m_scale.y, m_scale.z);
	m_worldPos.ApplyRotation(m_rot.x, 1,0,0);
	m_worldPos.ApplyRotation(m_rot.y, 0,1,0);
	m_worldPos.ApplyRotation(m_rot.z, 0,0,1);
	m_worldPos.ApplyTranslation(m_pos.x + m_translation.x, m_pos.y + m_translation.y, m_pos.z + m_translation.z);

	return m_worldPos;
}

