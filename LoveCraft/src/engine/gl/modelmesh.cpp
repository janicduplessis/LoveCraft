#include "modelmesh.h"
#include <cassert>

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4


void ModelMesh::VertexBoneData::AddBoneData(uint32 boneID, float weight)
{
	for(uint32 i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); ++i) {
		if (Weights[i] == 0.0) {
			IDs[i] = boneID;
			Weights[i] = weight;
			return;
		}
	}

	// too many bones
	assert(0);
}

ModelMesh::ModelMesh() : m_scale(1,1,1), m_translation(0,0,0), m_rot(0,0,0), m_pos(0,0,0), m_VAO(0),
						 m_numBones(0), m_scene(0) 
{
	ZERO_MEM(m_buffers);
}


ModelMesh::~ModelMesh()
{
	Clear();
}

bool ModelMesh::LoadMesh( const std::string& filename, bool flipUV )
{
	// Delete l'ancien mesh
	Clear();

	// Genere le vertex array
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Genere les buffers
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_buffers), m_buffers);

	bool ret = false;

	unsigned int postProcess = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace;
	if (flipUV)
		postProcess |= aiProcess_FlipUVs;

	m_scene = m_importer.ReadFile(filename.c_str(), postProcess);

	if (m_scene) {
		m_globalInverseTransform = m_scene->mRootNode->mTransformation.Inverse();
		ret = InitFromScene(m_scene, filename);
	}
	else {
		std::cout << "Error parsing " << filename.c_str() << " : " << m_importer.GetErrorString() << std::endl;
	}

	// Unbind le vertex array pour qu'il ne soit pas modifié de l'extérieur
	glBindVertexArray(0);

	return ret;
}

bool ModelMesh::InitFromScene( const aiScene* pScene, const std::string& Filename )
{
	m_entries.resize(pScene->mNumMeshes);
	m_textures.resize(pScene->mNumMaterials);

	std::vector<Vector3f> positions;
	std::vector<Vector3f> normals;
	std::vector<Vector2f> texCoords;
	std::vector<VertexBoneData> bones;
	std::vector<uint32> indices;

	uint32 numVertices = 0;
	uint32 numIndices = 0;

	// Count the number of vertices and indices
	for (uint32 i = 0 ; i < m_entries.size() ; i++) {
		m_entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;        
		m_entries[i].NumIndices    = pScene->mMeshes[i]->mNumFaces * 3;
		m_entries[i].BaseVertex    = numVertices;
		m_entries[i].BaseIndex     = numIndices;

		numVertices += pScene->mMeshes[i]->mNumVertices;
		numIndices  += m_entries[i].NumIndices;
	}

	// Reserve space in the vectors for the vertex attributes and indices
	positions.reserve(numVertices);
	normals.reserve(numVertices);
	texCoords.reserve(numVertices);
	bones.resize(numVertices);
	indices.reserve(numIndices);

	// Initialise tout les mesh du modele
	for (unsigned int i = 0 ; i < m_entries.size() ; i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh, positions, normals, texCoords, bones, indices);
	}

	if  (!InitMaterials(pScene, Filename))
		return false;

	// Generate and populate the buffers with vertex attributes and the indices
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB_POS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);    

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB_TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB_NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB_BONE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(BONE_ID_LOCATION);
	glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);    
	glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[VB_INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	CHECK_GL_ERROR();

	return true;
}

void ModelMesh::InitMesh( uint32 meshIndex, 
						 const aiMesh* paiMesh,
						 std::vector<Vector3f>& positions,
						 std::vector<Vector3f>& normals,
						 std::vector<Vector2f>& texCoords,
						 std::vector<VertexBoneData>& bones,
						 std::vector<uint32>& indices )
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (uint32 i = 0 ; i < paiMesh->mNumVertices ; i++) {
		const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
		normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
		texCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));        
	}

	LoadBones(meshIndex, paiMesh, bones);

	// Populate the index buffer
	for (uint32 i = 0 ; i < paiMesh->mNumFaces ; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		indices.push_back(Face.mIndices[0]);
		indices.push_back(Face.mIndices[1]);
		indices.push_back(Face.mIndices[2]);
	}
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


void ModelMesh::LoadBones( uint32 meshIndex, const aiMesh* paiMesh, std::vector<VertexBoneData>& bones )
{
	for(uint32 i = 0; i < paiMesh->mNumBones; ++i) {
		uint32 boneIndex = 0;
		std::string boneName(paiMesh->mBones[i]->mName.data);

		if(m_boneMapping.find(boneName) == m_boneMapping.end()) {
			boneIndex = m_numBones;
			m_numBones++;
			BoneInfo bi;
			m_boneInfo.push_back(bi);
		}
		else {
			boneIndex = m_boneMapping[boneName];
		}

		m_boneMapping[boneName] = boneIndex;
		m_boneInfo[boneIndex].BoneOffset = paiMesh->mBones[i]->mOffsetMatrix;

		for(uint32 j = 0; j < paiMesh->mBones[i]->mNumWeights; ++j) {
			uint32 vertexID = m_entries[meshIndex].BaseVertex + paiMesh->mBones[i]->mWeights[j].mVertexId;
			float weight = paiMesh->mBones[i]->mWeights[j].mWeight;
			bones[vertexID].AddBoneData(boneIndex, weight);
		}
	}
}

void ModelMesh::Render()
{
	glBindVertexArray(m_VAO);

	for (uint32 i = 0 ; i < m_entries.size() ; i++) {
		const uint32 materialIndex = m_entries[i].MaterialIndex;

		if (materialIndex < m_textures.size() && m_textures[materialIndex]) {
			m_textures[materialIndex]->Bind(GL_TEXTURE0);
		}

		glDrawElementsBaseVertex(GL_TRIANGLES, 
			m_entries[i].NumIndices, 
			GL_UNSIGNED_INT, 
			(void*)(sizeof(uint32) * m_entries[i].BaseIndex), 
			m_entries[i].BaseVertex);
	}

	glBindVertexArray(0);
}


void ModelMesh::BoneTransform( float timeInSeconds, std::vector<Matrix4f>& transforms )
{
	Matrix4f identity = Matrix4f::IDENTITY;

	float ticksPerSecond = m_scene->mAnimations[0]->mTicksPerSecond != 0 ?
						   m_scene->mAnimations[0]->mTicksPerSecond : 25.0f;
	float timeInTicks = timeInSeconds * ticksPerSecond;
	float animationTime = fmod(timeInTicks, m_scene->mAnimations[0]->mDuration);

	ReadNodeHeirarchy(animationTime, m_scene->mRootNode, identity);

	transforms.resize(m_numBones);

	for(uint32 i = 0; i < m_numBones; ++i) {
		transforms[i] = m_boneInfo[i].FinalTransformation;
	}
}


void ModelMesh::ReadNodeHeirarchy( float animationTime, const aiNode* pNode, const Matrix4f& parentTransform )
{
	std::string nodeName(pNode->mName.data);

	const aiAnimation* pAnimation = m_scene->mAnimations[0];

	Matrix4f nodeTransformation(pNode->mTransformation);

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, nodeName);

	if (pNodeAnim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D scaling;
		CalcInterpolatedScaling(scaling, animationTime, pNodeAnim);
		Matrix4f scalingM = Matrix4f::IDENTITY;;
		scalingM.ApplyScale(scaling.x, scaling.y, scaling.z);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion rotationQ;
		CalcInterpolatedRotation(rotationQ, animationTime, pNodeAnim); 
		Matrix4f rotationM = Matrix4f(rotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D translation;
		CalcInterpolatedPosition(translation, animationTime, pNodeAnim);
		Matrix4f translationM = Matrix4f::IDENTITY;
		translationM.ApplyTranslation(translation.x, translation.y, translation.z);

		// Combine the above transformations
		nodeTransformation = translationM * rotationM * scalingM;
	}

	Matrix4f globaltransformation = parentTransform * nodeTransformation;

	if(m_boneMapping.find(nodeName) != m_boneMapping.end()) {
		uint32 boneIndex = m_boneMapping[nodeName];
		m_boneInfo[boneIndex].FinalTransformation = m_globalInverseTransform * 
			globaltransformation * m_boneInfo[boneIndex].BoneOffset;
	}

	for(uint32 i = 0; i < pNode->mNumChildren; ++i) {
		ReadNodeHeirarchy(animationTime, pNode->mChildren[i], globaltransformation);
	}
}

void ModelMesh::CalcInterpolatedRotation( aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim )
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	uint32 RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	uint32 NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void ModelMesh::CalcInterpolatedPosition( aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim )
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	uint32 PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	uint32 NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void ModelMesh::CalcInterpolatedScaling( aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim )
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	uint32 ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	uint32 NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

uint32 ModelMesh::FindRotation( float AnimationTime, const aiNodeAnim* pNodeAnim )
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (uint32 i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);
}

uint32 ModelMesh::FindScaling( float AnimationTime, const aiNodeAnim* pNodeAnim )
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (uint32 i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}

uint32 ModelMesh::FindPosition( float AnimationTime, const aiNodeAnim* pNodeAnim )
{
	for (uint32 i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}

const aiNodeAnim* ModelMesh::FindNodeAnim( const aiAnimation* pAnimation, const string NodeName )
{
	for (uint32 i = 0 ; i < pAnimation->mNumChannels ; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}

void ModelMesh::Clear()
{
	for (unsigned int i = 0 ; i < m_textures.size() ; i++) {
		delete m_textures[i];
	}

	if (m_buffers[0] != 0) {
		glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_buffers), m_buffers);
	}

	if (m_VAO != 0) {
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
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
	
	m_worldPos.ApplyTranslation(m_pos.x + m_translation.x, m_pos.y + m_translation.y, m_pos.z + m_translation.z);
	m_worldPos.ApplyRotation(m_rot.x, 1,0,0);
	m_worldPos.ApplyRotation(m_rot.y, 0,1,0);
	m_worldPos.ApplyRotation(m_rot.z, 0,0,1);
	m_worldPos.ApplyScale(m_scale.x, m_scale.y, m_scale.z);

	return m_worldPos;
}


