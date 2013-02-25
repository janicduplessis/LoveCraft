#include "skybox.h"
#include <iostream>


Skybox::Skybox() : m_cubemapTexture(0), m_skyboxShader(0), m_skyboxMesh(0) {}

Skybox::~Skybox()
{
	delete m_cubemapTexture;
	delete m_skyboxMesh;
	delete m_skyboxShader;
}

bool Skybox::Init( const string& Directory, const string& PosXFilename, const string& NegXFilename, const string& PosYFilename, const string& NegYFilename, const string& PosZFilename, const string& NegZFilename )
{
	m_cubemapTexture = new CubeMapTexture(Directory, PosXFilename, NegXFilename, PosYFilename, NegYFilename, PosZFilename, NegZFilename);
	if (!m_cubemapTexture->Load()) {
		std::cout << "Failed to load skybox texture" << std::endl;
		return false;
	}
	m_skyboxMesh = new ModelMesh;
	if (!m_skyboxMesh->LoadMesh(SKYBOX_PATH "sphere.obj", true)) {
		std::cout << "Failed to load skybox mesh" << std::endl;
		return false;
	}

	m_skyboxMesh->Scale(Vector3f(10,10,10));

	m_skyboxShader = new SkyboxShader;
	if (!m_skyboxShader->Init()) {
		std::cout << "Failed to load skybox shader program" << std::endl;
		return false;
	}
	m_skyboxShader->Use();
	m_skyboxShader->SetTextureUnit(0);
	Shader::Disable();
	CHECK_GL_ERROR();
	return true;
}

void Skybox::Render( const Matrix4f& wvp )
{
	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	CHECK_GL_ERROR();

	m_cubemapTexture->Bind(GL_TEXTURE0);
	m_skyboxShader->Use();
	m_skyboxShader->SetWVP(wvp);

	m_skyboxMesh->Render();

	Shader::Disable();

	glCullFace(OldCullFaceMode);        
	glDepthFunc(OldDepthFuncMode);
}
