#ifndef PARTICLES_H__
#define PARTICLES_H__

#include "texture.h"
#include "mesh.h"

#include <util/vector3.h>
#include <util/quaternion.h>

/**
 * @brief Particles engine
 */
class Particles : Mesh
{
private:
	/**
	 * Une particule
	 */
	struct Particle
	{
		Vector3f pos; // Position
		Vector3f velocity; // Vitesse initiale
		Vector3f color; // Couleur
		float timeAlive; // Temps depuis son initialisation
		float lifespan; // Durée de vie
	};

	/**
	 * Vertex data avec couleur, alpha et texture
	 */
	struct VertexData
	{
		float x, y, z;
		float r, g, b, a;
		float u, v;

		VertexData() {}
		VertexData(float _x, float _y, float _z, float _r, float _g, float _b, float _a, float _u, float _v) : 
			x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), a(_a), u(_u), v(_v) {}
	};

public:
	/**
	 * Constructeur
	 * @param particlesNumber Nombre de particules
	 */
	Particles(unsigned int particlesNumber);
	~Particles();

	/**
	 * Initialise le moteur de particules
	 */
	void Init();
	/**
	 * Render les particules
	 */
	void Render(bool wireFrame = false) const;
	/**
	 * Update la position des particules
	 * Doit être appelé a chaque frame
	 */
	void Update(float elapsedTime);

	/**
	 * Position du lanceur de particules
	 */
	void SetPosition(const Vector3f& pos);
	/**
	 * Angle de projection des particules
	 */
	void SetRotation(const Quaternion& q);
	/**
	 * Nombre de particules (NYI)
	 */
	void SetParticlesNumber(float nbr);
	/**
	 * Degré de variation de l'angle de projection
	 * des particules
	 */
	void SetRange(float range);

	/**
	 * Dimention des particules
	 */
	void SetParticlesSize(float size);
	/**
	 * Texture des particules
	 */
	void SetTexture(Texture* texture);
	/**
	 * Couleur des particules
	 */
	void SetColor(const Vector3f& color);
	/**
	 * Temps de vie moyen des particules
	 */
	void SetAverageLifespan(float lifespan);
	/**
	 * Vitesse initiale moyenne des particules
	 */
	void SetAverageVelocity(float velocity);

private:
	// Float entre 0 et 1
	float RandomFloat() const;
	Vector3f AvgVelocity() const;
	void CreateParticle(Particle* p) const;
	void SetMeshData(VertexData* vd, unsigned int vertexCount);
	// Compare 2 particules pour savoir laquelle est la plus proche de la caméra
	static bool CompareParticles(Particle* particle1, Particle* particle2);
	//NYI
	void FaceCamera(Particle* p);
private:
	Particle* m_particles;

	// Propriétés des particules
	float m_particlesSize;
	float m_averageVelocity;
	float m_averageLifespan;
	Vector3f m_color;
	Texture* m_texture;

	// Propriétés lanceur de particules
	Quaternion m_angle;
	Vector3f m_pos;
	unsigned int m_particlesNumber;
	float m_range;
};

#endif