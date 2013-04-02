#ifndef SKYBOX_H_
#define SKYBOX_H_

#include "define.h"
#include "shaders/skyboxshader.h"
#include "cubemaptexture.h"
#include "modelmesh.h"
#include "engine/pipeline.h"

class Skybox
{
public:
	Skybox();
	~Skybox();

	bool Init(const string& Directory,
		const string& PosXFilename,
		const string& NegXFilename,
		const string& PosYFilename,
		const string& NegYFilename,
		const string& PosZFilename,
		const string& NegZFilename);

	void Render(Pipeline p);

private: 
	SkyboxShader* m_skyboxShader;
	CubeMapTexture* m_cubemapTexture;
	ModelMesh* m_skyboxMesh;
};

#endif