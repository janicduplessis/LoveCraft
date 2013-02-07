#include "define.h"
#include "util/vector3.h"
#include "util/vector2.h"
#include "util/matrix4.h"
#include "texture.h"

#include <map>
#include <vector>

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>       
#include <assimp/postprocess.h>

#define INVALID_MATERIAL 0xFFFFFFFF
#define INVALID_OGL_VALUE 0xFFFFFFFF

struct Vertex
{
	Vector3f m_pos;
	Vector3f m_tex;
	Vector3f m_normal;

	Vertex() {}

	Vertex(const Vector3f& pos, const Vector3f& tex, const Vector3f& normal)
	{
		m_pos    = pos;
		m_tex    = tex;
		m_normal = normal;
	}
};

class ModelMesh
{
public:
	ModelMesh();
	~ModelMesh();

	bool LoadMesh(const std::string& Filename);

	void Translate( const Vector3f& trans );
	void Scale( const Vector3f& scale );
	void SetPosition(const Vector3f& pos);
	void SetRotation(const Vector3f& rot);

	Matrix4f GetWorldMatrix();

	void Render();

private:
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();
	


	struct MeshEntry {
		MeshEntry();

		~MeshEntry();

		bool Init(const std::vector<Vertex>& Vertices,
			const std::vector<unsigned int>& Indices);

		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

	std::vector<MeshEntry> m_entries;
	std::vector<Texture*> m_textures;

	Matrix4f m_worldPos;
	Vector3f m_rot;
	Vector3f m_pos;
	Vector3f m_scale;
	Vector3f m_translation;
};