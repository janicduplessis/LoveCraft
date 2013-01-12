#ifndef PS_UPDATE_SHADER_H__
#define PS_UPDATE_SHADER_H__

#include "shader.h"
#include "define.h"

class PSUpdateShader : public Shader
{
public:
	PSUpdateShader();
	~PSUpdateShader();

	virtual bool Init();    

	void SetDeltaTimeMillis(int deltaTimeMilli);

	void SetTime(int time);

	void SetRandomTextureUnit(unsigned int textureUnit);

	void SetLauncherLifetime(float lifetime);

	void SetShellLifetime(float lifetime);

	void SetSecondaryShellLifetime(float lifetime);

private:
	GLuint m_deltaTimeMillisLocation;
	GLuint m_randomTextureLocation;
	GLuint m_timeLocation;
	GLuint m_launcherLifetimeLocation;
	GLuint m_shellLifetimeLocation;
	GLuint m_secondaryShellLifetimeLocation;
};


#endif