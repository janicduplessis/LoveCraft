#ifndef SKYBOX_H_
#define SKYBOX_H_

#include "chunkmesh.h"
#include "define.h"
#include "util/vector3.h"
#include "engine/info.h"
#include "engine/gl/texturearray.h"

class Skybox
{
public:
	enum Sides
	{
		S_LEFT,
		S_CENTER,
		S_RIGHT,
		S_BACK,
		S_TOP,
		S_BOTTOM
	};
	Skybox();
	~Skybox();
	void Init();
	void Update(Vector3f &playerpos = Vector3f());
	void Render();
private:
	ChunkMesh m_mesh;
	TextureArray m_texArray;
	Vector3f m_playerpos;
};

#endif