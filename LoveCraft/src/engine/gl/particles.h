#ifndef PARTICLES_H__
#define PARTICLES_H__

#include "texture.h"
#include "mesh.h"
#include "randomtexture.h"
#include "shaders/billboardshader.h"
#include "shaders/psupdateshader.h"

#include <util/vector3.h>
#include <util/quaternion.h>

/**
* @brief Particles engine
*/
class Particles
{
private:
	/**
	* Une particule
	*/
	struct Particle
	{
		float type;
		Vector3f pos; // Position
		Vector3f velocity; // Vitesse initiale
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
	//Particles(const Particles& p);
	~Particles();

	/**
	* Initialise le moteur de particules
	*/
	bool Init();
	/**
	* Render les particules
	*/
	void Render(const Matrix4f& VP, bool wireFrame = false);
	/**
	* Update la position des particules
	* Doit être appelé a chaque frame
	*/
	void Update(int deltaTimeMilli);

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
	Vector3f AvgVelocity() const;

private:
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

	bool m_isFirst;

	GLuint m_particleBuffer[2];
	GLuint m_transformFeedback[2];

	unsigned int m_currVB;
	unsigned int m_currTFB;

	int m_time;

	BillboardShader m_billboardShader;
	PSUpdateShader m_updateShader;
	RandomTexture m_randomTexture;
};

#endif