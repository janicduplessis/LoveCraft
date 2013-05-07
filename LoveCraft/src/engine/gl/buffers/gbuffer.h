#ifndef GBUFFER_H__
#define GBUFFER_H__

#include "define.h"

class GBuffer
{
public:
	enum GBUFFER_TEXTURE_TYPE
	{
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_NUM_TEXTURES
	};

	GBuffer();
	~GBuffer();

	bool Init(uint32 windowWidth, uint32 windowHeight);

	void StartFrame();
	void DrawBind();
	void BindForGeometryPass();
	void BindForStencilPass();
	void BindForLightPass();
	void BindForFinalPass();

private:
	GLuint m_fbo;
	GLuint m_textures[GBUFFER_NUM_TEXTURES];
	GLuint m_depthTexture;
	GLuint m_finalTexture;
};


#endif // !GBUFFER_H__
