#ifndef LIGHTING_SHADER_H__
#define LIGHTING_SHADER_H__

#include "shader.h"

#include <util/vector3.h>
#include <util/matrix4.h>

#define MAX_POINT_LIGHTS 3

/**
 * Structure de base d'une light
 */
struct BaseLight
{
	Vector3f Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight() : Color(0), AmbientIntensity(0), DiffuseIntensity(0) {}
};

/**
 * Directional Light
 * Light avec seulement une direction,
 * appliqué également partout
 */
struct DirectionalLight : public BaseLight
{
	Vector3f Direction;

	DirectionalLight() : BaseLight(), Direction(0) {}
};

/**
 * Point Light
 * Light avec une position et une atténuation
 * Son intensité diminue plus on est loin de sa position
 */
struct PointLight : public BaseLight
{
	Vector3f Position;

	struct {
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

	PointLight() : BaseLight(), Position(0) {
		Attenuation.Constant = 0;
		Attenuation.Linear = 0;
		Attenuation.Exp = 0;
	}
};

/**
 * Shader pour calculer l'éclairage
 */
class LightingShader : public Shader
{
public:
	LightingShader();
	~LightingShader();

	virtual bool Init();

	// Propriétés globales
	void SetWVP(const Matrix4f& WVP);
	void SetWorld(const Matrix4f& world);
	void SetEyeWorldPos(const Vector3f& eyePos);
	void SetTextureUnitType(int type);
	void SetColorTextureUnit(unsigned int textureUnit);
	void SetNormalTextureUnit(unsigned int textureUnit);

	// Directional lighting
	void SetDirectionalLight(const DirectionalLight& light);

	// Point lights
	void SetPointLights(unsigned int numLights, const PointLight* pLights);
	void UpdatePointLight(unsigned int id, const PointLight& pLight);

	// Proprétés du specular lighting
	void SetMatSpecualarIntensity(float intensity);
	void SetMatSpecularPower(float power);

private:
	// ----------------------------------------------------------------------
	// Program uniform locations
	GLuint m_WVPLocation;					// World viewport
	GLuint m_worldLocation;					// World position
	GLuint m_eyeWorldPosLocation;			// Camera position
	GLuint m_matSpecularIntensityLocation;	// Material specular intensity
	GLuint m_matSpecularPowerLocation;		// Material specular power

	GLuint m_samplerTypeLocation;			// Type du sampler 0 = texture, 1 = array
	GLuint m_colorSamplerLocation;			// Color texture sampler
	GLuint m_normalSamplerLocation;			// Normal texture sampler
	GLuint m_arraySamplerLocation;			// Texture array sampler

	// Directional light location struct
	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint Direction;
		GLuint DiffuseIntensity;
	} m_dirLightLocation;

	// Point light location struct
	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		struct {
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_pointLightsLocation[MAX_POINT_LIGHTS];
	GLuint m_numPointLightsLocation;
};

#endif