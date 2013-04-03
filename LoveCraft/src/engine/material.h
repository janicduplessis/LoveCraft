#ifndef MATERIAL_H__
#define MATERIAL_H__

#include "define.h"
#include "gl/texture.h"

class Material
{
public:
	Material();
	Material(Texture* diffuse, Texture* normal);
	Material(Texture* diffuse, Texture* normal, float intensity, float power);
	~Material();

	void Bind();

	bool LoadDiffuseMap(const std::string& path);
	Texture* GetDiffuseMap() const;
	void SetDiffuseMap(Texture* val);

	bool LoadNormalMap(const std::string& path);
	Texture* GetNormalMap() const;
	void SetNormalMap(Texture* val);

	float GetSpecularIntensity() const;
	void SetSpecularIntensity(float val);

	float GetSpecularPower() const;
	void SetSpecularPower(float val);

private:
	Texture* m_diffuseMap;
	Texture* m_normalMap;
	
	float m_specularIntensity;
	float m_specularPower;
};

#endif