#ifndef MODEL_H__
#define MODEL_H__

#include "mesh.h"
#include "util/vector2.h"
#include "util/vector3.h"
#include "util/quaternion.h"

#include <string>

#define MAX_BONES_PER_VERTEX 4

/**
 * @brief Modèle loadé a partir d'un fichier lcm (LoveCraft Model)
 * 
 * Utiliser modelimport.exe pour convertir la plupart des format de model
 * en lcm
 */
class Model
{
public:
	// Structure représentant toutes les informations de chacuns des vertex
	// S'assurer que le size de cette struct reste un multiple de 32
	// octet pour un maximum de performance
	// Au besoin, ajouter du padding
	struct VertexData
	{
		float x, y, z;
		float r, g, b;
		float u, v;
		float n1, n2, n3;

		VertexData() {}
		VertexData(float _x, float _y, float _z, float _r, float _g, float _b, float _u, float _v) : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), u(_u), v(_v) {}
	};

	struct VertexBoneData
	{
		uint16 IDs[MAX_BONES_PER_VERTEX];
		float Weights[MAX_BONES_PER_VERTEX];
	};

	struct Mesh
	{
		VertexData* vertices;
		uint32 verticesCount;
		GLuint verticesVboId;
		uint16* indices;
		uint32 indicesCount;
		GLuint indicesVboId;
	};

public:
	/**
	 * Constructeur
	 */
	Model();
	/**
	 * Déconstructeur
	 */
	virtual ~Model();

	/**
	 * Load un modèle lcm depuis un fichier
	 */
	virtual bool Load(const std::string filePath);

	/**
	 * Render le modèle
	 */
	virtual void Render(bool wireFrame = false) const;
	/**
	 * Copie le mesh dans la carte graphique
	 */
	virtual void SetMeshData(Mesh* meshes, uint32 meshesCount);

	/**
	 * Déplace le modèle par rapport à son origine
	 */
	virtual void Translate(float x, float y, float z);
	virtual void Translate(const Vector3f& trans);
	/**
	 * Tourne le modèle autour de son origine
	 */
	virtual void Rotate(const Quaternion& rot);
	virtual Quaternion GetRotation() const;
	virtual void SetRotation(const Vector3f& rot);
	virtual void SetRotation(const Quaternion& rot);
	
	/**
	 * Scale le modèle
	 */
	virtual void Scale(const Vector3f& scale);
	virtual void Scale(float x, float y, float z);

	/**
	 * Position de l'origine
	 */
	virtual Vector3f GetPosition() const;
	virtual void SetPosition(const Vector3f& pos);
	

private:
	Vector3f m_pos;
	Vector3f m_translation;
	Quaternion m_rot;
	Vector3f m_scale;

	Mesh* m_meshes;
	uint32 m_meshCount;

	GLuint m_bonesVboId;
};

#endif