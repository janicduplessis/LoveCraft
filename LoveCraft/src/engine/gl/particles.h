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
	struct Particle
	{
		Vector3f pos;
		Vector3f velocity;
		Vector3f color;
		float timeAlive;
		float lifespan;
	};

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
	Particles(unsigned int particlesNumber);
	~Particles();

	void Init();
	void Render(bool wireFrame = false) const;
	void Update(float elapsedTime);

	void SetPosition(const Vector3f& pos);
	void SetRotation(const Quaternion& q);
	void SetParticlesNumber(float nbr);
	void SetRange(float range);

	void SetParticlesSize(float size);
	void SetTexture(Texture* texture);
	void SetColor(const Vector3f& color);
	void SetAverageLifespan(float lifespan);
	void SetAverageVelocity(float velocity);

private:
	float RandomFloat() const;
	Vector3f AvgVelocity() const;
	void CreateParticle(Particle* p) const;
	void SetMeshData(VertexData* vd, unsigned int vertexCount);
	static bool CompareParticles(Particle* particle1, Particle* particle2);
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