#ifndef LIGHTING_SHADER_H__
#define LIGHTING_SHADER_H__

#include "define.h"

#include "../shader.h"
#include "lights.h"

#include "util/matrix4.h"
#include "util/vector3.h"

/**
 * Shader pour calculer l'éclairage
 */
class LightingShader : public Shader
{
public:
	static const unsigned int MAX_POINT_LIGHTS = 3;

	LightingShader();
	~LightingShader();

	virtual bool Init();

	// Propriétés globales
	void SetWVP(const Matrix4f& WVP);
	void SetWorld(const Matrix4f& world);
	void SetEyeWorldPos(const Vector3f& eyePos);
	void SetTextureUnitType(int type);
	void SetColorTextureUnit(unsigned int textureUnit);

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
	GLuint m_arraySamplerLocation;			// Texture array sampler
	GLuint m_colorSamplerLocation;
	GLuint m_normalSamplerLocation;

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