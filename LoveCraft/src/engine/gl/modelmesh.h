#ifndef MODELMESH_H__
#define MODELMESH_H__

#include <map>
#include <vector>

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>       
#include <assimp/postprocess.h>

#include "define.h"
#include "shaders/modelshader.h"
#include "util/vector3.h"
#include "util/vector2.h"
#include "util/matrix4.h"
#include "engine/material.h"


#define INVALID_MATERIAL 0xFFFFFFFF
#define INVALID_OGL_VALUE 0xFFFFFFFF


class ModelMesh
{
public:
	ModelMesh();
	~ModelMesh();

	bool Init(const std::string& Filename, ModelShader* shader, bool flipUV = false);

	void Render();

	uint32 NumBones() const {
		return m_numBones;
	}

	void BoneTransform(float timeInSeconds, std::vector<Matrix4f>& transforms);

	void LoadDefaultMaterials(bool val);

private:

	#define MAX_BONES_PER_VERTEX 4

	struct BoneInfo
	{
		Matrix4f BoneOffset;
		Matrix4f FinalTransformation;

		BoneInfo()
		{
			BoneOffset.SetZero();
			FinalTransformation.SetZero();
		}
	};

	struct VertexBoneData
	{
		uint32 IDs[MAX_BONES_PER_VERTEX];
		float Weights[MAX_BONES_PER_VERTEX];

		VertexBoneData()
		{
			Reset();
		}

		void Reset()
		{
			ZERO_MEM(IDs);
			ZERO_MEM(Weights);
		}

		void AddBoneData(uint32 boneID, float weight);
	};

	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);    
	uint32 FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	uint32 FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	uint32 FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
	void ReadNodeHeirarchy(float animationTime, const aiNode* pNode, const Matrix4f& parentTransform);

	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(uint32 meshIndex, 
				  const aiMesh* paiMesh,
				  std::vector<Vector3f>& positions,
				  std::vector<Vector3f>& normals,
				  std::vector<Vector3f>& tangents,
				  std::vector<Vector2f>& texCoords,
				  std::vector<VertexBoneData>& bones,
				  std::vector<uint32>& indices);
	void LoadBones(uint32 meshIndex, const aiMesh* paiMesh, std::vector<VertexBoneData>& bones);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

	enum VB_TYPES {
		VB_INDEX,
		VB_POS,
		VB_NORMAL,
		VB_TANGENTS,
		VB_TEXCOORD,
		VB_BONE,
		VB_COUNT
	};
	
	GLuint m_VAO;
	GLuint m_buffers[VB_COUNT];

	struct MeshEntry {
		uint32 NumIndices;
		uint32 BaseVertex;
		uint32 BaseIndex;
		uint32 MaterialIndex;

		MeshEntry() : NumIndices(0), BaseVertex(0), BaseIndex(0), MaterialIndex(INVALID_MATERIAL) {}
	};

	std::vector<MeshEntry> m_entries;
	std::vector<Material> m_materials;

	std::map<std::string, uint32> m_boneMapping;
	uint32 m_numBones;
	std::vector<BoneInfo> m_boneInfo;
	Matrix4f m_globalInverseTransform;

	Assimp::Importer m_importer;
	const aiScene* m_scene;

	ModelShader* m_shader;

	bool m_loadDefaultMaterials;
};

#endif