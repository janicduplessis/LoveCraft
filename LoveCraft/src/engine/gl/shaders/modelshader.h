#ifndef MODEL_SHADER_H__
#define MODEL_SHADER_H__

#include "shadernew.h"
#include "lights.h"

/**
 * Shader pour calculer l'éclairage
 */
class ModelShader : public ShaderNew
{
public:
	static const uint32 MAX_POINT_LIGHTS = 3;
	static const uint32 MAX_BONES = 100;


	ModelShader();
	~ModelShader();

	virtual bool Init();

	// Propriétés globales
	void SetWVP(const Matrix4f& WVP);
	void SetWorld(const Matrix4f& world);
	void SetEyeWorldPos(const Vector3f& eyePos);

	// Directional lighting
	void SetDirectionalLight(const DirectionalLight& light);

	// Point lights
	void SetPointLights(unsigned int numLights, const PointLight* pLights);
	void UpdatePointLight(unsigned int id, const PointLight& pLight);

	// Proprétés du specular lighting
	void SetMatSpecualarIntensity(float intensity);
	void SetMatSpecularPower(float power);

	// Bones
	void SetBoneTransform(uint32 index, const Matrix4f& transform);

private:
	// ----------------------------------------------------------------------
	// Program uniform locations
	GLuint m_WVPLocation;					// World viewport
	GLuint m_worldLocation;					// World position
	GLuint m_eyeWorldPosLocation;			// Camera position
	GLuint m_matSpecularIntensityLocation;	// Material specular intensity
	GLuint m_matSpecularPowerLocation;		// Material specular power

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
	GLuint m_boneLocation[MAX_BONES];
};

#endif