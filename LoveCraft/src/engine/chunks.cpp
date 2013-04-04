#include "chunks.h"


Chunks::Chunks( uint32 x, uint32 y ) : Array2d<Chunk*>(x, y)
{

}

Chunks::~Chunks()
{
	// delete les chunks
	for (int i = 0; i < VIEW_DISTANCE / CHUNK_SIZE_X * 2 - 1; i++)
	{
		for (int j = 0; j < VIEW_DISTANCE / CHUNK_SIZE_Z * 2 - 1; ++j)
		{
			delete Get(i, j);
		}
	}
}

void Chunks::Render(Pipeline p)
{
	m_shader->Enable();
	m_shader->SetTextureUnitType(1);
	m_shader->SetWVP(p.GetWVPTrans());
	m_shader->SetWorld(p.GetWorldTrans());
	CHECK_GL_ERROR();
	for (int i = 0; i < VIEW_DISTANCE / CHUNK_SIZE_X * 2; i++)
	{
		for (int j = 0; j < VIEW_DISTANCE / CHUNK_SIZE_Z * 2; ++j)
		{
			Get(i,j)->Render();
			CHECK_GL_ERROR();
		}
	}

	Shader::Disable();
}

void Chunks::Update()
{
	int updated = 0;
	for (int i = 0; i < VIEW_DISTANCE / CHUNK_SIZE_X * 2; i++)
	{
		for (int j = 0; j < VIEW_DISTANCE / CHUNK_SIZE_Z * 2; ++j)
		{
			Chunk* c = Get(i,j);
			if (c->IsReady()) {
				if (c->IsDirty() && updated < 2) {
					c->Update();
					updated++;
				}
			}
		}
	}
}

void Chunks::Init( LightingShader* shader )
{
	for (int i = 0; i < VIEW_DISTANCE / CHUNK_SIZE_X * 2; ++i)
	{
		for (int j = 0; j < VIEW_DISTANCE / CHUNK_SIZE_Z * 2; ++j)
		{
			Chunk* c = new Chunk(Vector2i(i,j), Vector2f(i,j));
			Set(i, j, c);
		}
	}

	for (int i = 0; i < VIEW_DISTANCE / CHUNK_SIZE_X * 2; ++i)
	{
		for (int j = 0; j < VIEW_DISTANCE / CHUNK_SIZE_Z * 2; ++j)
		{
			Chunk* c = Get(i,j);
			c->SetSurroundings(Vector2i(i,j));
			c->GenerateTrees();
			c->SetIsReady(true);
		}
	}

	m_shader = shader;
}
