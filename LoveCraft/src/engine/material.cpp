#include "material.h"

Material::Material() : 
	m_diffuseMap(0), m_normalMap(0), m_specularIntensity(0.1f), m_specularPower(16)
{

}

Material::Material( Texture* diffuse, Texture* normal ) : 
	m_diffuseMap(diffuse), m_normalMap(normal), m_specularIntensity(0.1f), m_specularPower(16)
{

}

Material::Material( Texture* diffuse, Texture* normal, float intensity, float power ) : 
	m_diffuseMap(diffuse), m_normalMap(normal), m_specularIntensity(intensity), m_specularPower(power)
{

}

Material::~Material()
{
	delete m_normalMap;
	delete m_diffuseMap;
}

void Material::Bind()
{
	if (m_diffuseMap)
		m_diffuseMap->Bind(GL_TEXTURE0);
	if (m_normalMap)
		m_normalMap->Bind(GL_TEXTURE2);
}

void Material::SetSpecularPower( float val )
{
	m_specularPower = val;
}

float Material::GetSpecularPower() const
{
	return m_specularPower;
}

void Material::SetSpecularIntensity( float val )
{
	m_specularIntensity = val;
}

float Material::GetSpecularIntensity() const
{
	return m_specularIntensity;
}

void Material::SetNormalMap( Texture* val )
{
	m_normalMap = val;
}

Texture* Material::GetNormalMap() const
{
	return m_normalMap;
}

void Material::SetDiffuseMap( Texture* val )
{
	m_diffuseMap = val;
}

Texture* Material::GetDiffuseMap() const
{
	return m_diffuseMap;
}

bool Material::LoadDiffuseMap( const std::string& path )
{
	delete m_diffuseMap;
	m_diffuseMap = new Texture;
	if(!m_diffuseMap->Load(path)) {
		delete m_diffuseMap;
		m_diffuseMap = 0;
		return false;
	}

	return true;
}

bool Material::LoadNormalMap( const std::string& path )
{
	delete m_normalMap;
	m_normalMap = new Texture;
	if(!m_normalMap->Load(path)) {
		delete m_normalMap;
		m_normalMap = 0;
		return false;
	}

	return true;
}


