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


// Stores an edge by its vertices and force an order between them
struct Edge
{
	Edge(uint32 _a, uint32 _b)
	{
		assert(_a != _b);

		if (_a < _b)
		{
			a = _a;
			b = _b;                   
		}
		else
		{
			a = _b;
			b = _a;
		}
	}

	uint32 a;
	uint32 b;
};

struct Neighbors
{
	uint32 n1;
	uint32 n2;

	Neighbors()
	{
		n1 = n2 = (uint32)-1;
	}

	void AddNeigbor(uint32 n)
	{
		if (n1 == -1) {
			n1 = n;
		}
		else if (n2 == -1) {
			n2 = n;
		}
		else {
			assert(0);
		}
	}

	uint32 GetOther(uint32 me) const
	{
		if (n1 == me) {
			return n2;
		}
		else if (n2 == me) {
			return n1;
		}
		else {
			assert(0);
		}

		return 0;
	}
};

struct CompareEdges
{
	bool operator()(const Edge& Edge1, const Edge& Edge2)
	{
		if (Edge1.a < Edge2.a) {
			return true;
		}
		else if (Edge1.a == Edge2.a) {
			return (Edge1.b < Edge2.b);
		}        
		else {
			return false;
		}            
	}
};

struct ComparePositions
{
	bool operator()(const aiVector3D& pos1, const aiVector3D& pos2)
	{
		if (pos1.x < pos2.x) {
			return true;
		}
		else if (pos1.x == pos2.x) {
			if (pos1.y < pos2.y) {
				return true;
			}
			else if (pos1.y == pos2.y) {
				return (pos1.z < pos2.z);
			}
			else {
				return false;
			}
		}        
		else {
			return false;
		}          
	}
};

struct Face
{
	uint32 Indices[3];
	uint32 GetOppositeIndex( const Edge& e ) const;
};


class ModelMesh
{
public:
	ModelMesh();
	~ModelMesh();

	bool Init(const std::string& Filename, bool withAdjacencies, BoneModelShader* shader, bool flipUV = false);

	void Render();
	void RenderDepth();
	void RenderShadowVolume();

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
	void FindAdjacencies(const aiMesh* paiMesh, std::vector<uint32>& indices);

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

	BoneModelShader* m_shader;

	std::map<Edge, Neighbors, CompareEdges> m_indexMap;
	std::map<aiVector3D, uint32, ComparePositions> m_posMap;
	std::vector<Face> m_uniqueFaces;
	bool m_withAdjacencies;

	bool m_loadDefaultMaterials;
};

#endif