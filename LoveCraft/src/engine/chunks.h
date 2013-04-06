#ifndef CHUNKS_H__
#define CHUNKS_H__

#include "define.h"
#include "pipeline.h"
#include "util/array2d.h"
#include "chunk.h"
#include "gl/shaders/lightingshader.h"
#include "gl/shaders/nullshader.h"

class Chunks : public Array2d<Chunk*>
{
public:
	Chunks(uint32 x, uint32 y);
	virtual ~Chunks();

	void Init(LightingShader* shader);

	void Render(Pipeline p);
	void Render(Pipeline p, StaticNullShader* nullShader);
	void Update();

private:
	LightingShader* m_shader;
};

#endif // !CHUNKS_H__
