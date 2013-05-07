#ifndef CHUNKS_H__
#define CHUNKS_H__

#include "define.h"
#include "pipeline.h"
#include "util/array2d.h"
#include "chunk.h"
#include "gl/shaders/lighting/lighting_shader.h"
#include "gl/shaders/null/static_bone_shader.h"
#include "gl/shaders/shadow/static_shadow_volume_shader.h"

class Chunks : public Array2d<Chunk*>
{
public:
	Chunks(uint32 x, uint32 y);
	virtual ~Chunks();

	void Init(LightingShader* shader);

	void Render(Pipeline p);
	void Render(Pipeline p, StaticNullShader* nullShader);
	void Render(Pipeline p, StaticShadowVolumeShader* shadowShader);
	void Update();

private:
	LightingShader* m_shader;
};

#endif // !CHUNKS_H__
