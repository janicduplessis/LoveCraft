#include "skybox.h"


Skybox::Skybox() : m_playerpos(Vector3f())
{
	m_mesh = new ChunkMesh(Info::Get().GetCubeShader());
	m_data = new ChunkMesh::VertexData[24];
	m_texData = new ChunkMesh::TextureData[24];
	m_texArray = new TextureArray(6);
}


Skybox::~Skybox()
{
	delete m_mesh;
	delete m_data;
	delete m_texArray;
	delete m_texData;
}

void Skybox::Init()
{
	Vector3f mx_my_mz = Vector3f(-1, -1, -1);
	Vector3f x_my_mz = Vector3f(1, -1, -1);
	Vector3f mx_y_mz = Vector3f(-1, 1, -1);
	Vector3f x_y_mz = Vector3f(1, 1, -1);
	Vector3f mx_my_z = Vector3f(-1, -1, 1);
	Vector3f x_my_z = Vector3f(1, -1, 1);
	Vector3f mx_y_z = Vector3f(-1, 1, 1);
	Vector3f x_y_z = Vector3f(1, 1, 1);

	Vector2i tl = Vector2i(-1, 1);
	Vector2i tr = Vector2i(1, 1);
	Vector2i bl = Vector2i(-1, -1);
	Vector2i br = Vector2i(1, -1);

	m_data[S_LEFT + 0] = ChunkMesh::VertexData(mx_my_mz * VIEW_DISTANCE);
	m_data[S_LEFT + 1] = ChunkMesh::VertexData(mx_my_z * VIEW_DISTANCE);
	m_data[S_LEFT + 2] = ChunkMesh::VertexData(mx_y_z * VIEW_DISTANCE);
	m_data[S_LEFT + 3] = ChunkMesh::VertexData(mx_y_mz * VIEW_DISTANCE);

	m_data[S_CENTER + 0] = ChunkMesh::VertexData(mx_my_z * VIEW_DISTANCE);
	m_data[S_CENTER + 1] = ChunkMesh::VertexData(x_my_z * VIEW_DISTANCE);
	m_data[S_CENTER + 2] = ChunkMesh::VertexData(x_y_z * VIEW_DISTANCE);
	m_data[S_CENTER + 3] = ChunkMesh::VertexData(mx_y_z * VIEW_DISTANCE);

	m_data[S_RIGHT + 0] = ChunkMesh::VertexData(x_my_z * VIEW_DISTANCE);
	m_data[S_RIGHT + 1] = ChunkMesh::VertexData(x_my_mz * VIEW_DISTANCE);
	m_data[S_RIGHT + 2] = ChunkMesh::VertexData(x_y_mz * VIEW_DISTANCE);
	m_data[S_RIGHT + 3] = ChunkMesh::VertexData(x_y_z * VIEW_DISTANCE);

	m_data[S_BACK + 0] = ChunkMesh::VertexData(x_my_mz * VIEW_DISTANCE);
	m_data[S_BACK + 1] = ChunkMesh::VertexData(mx_my_mz * VIEW_DISTANCE);
	m_data[S_BACK + 2] = ChunkMesh::VertexData(mx_y_mz * VIEW_DISTANCE);
	m_data[S_BACK + 3] = ChunkMesh::VertexData(x_y_mz * VIEW_DISTANCE);

	m_data[S_TOP + 0] = ChunkMesh::VertexData(mx_y_z * VIEW_DISTANCE);
	m_data[S_TOP + 1] = ChunkMesh::VertexData(x_y_z * VIEW_DISTANCE);
	m_data[S_TOP + 2] = ChunkMesh::VertexData(x_y_mz * VIEW_DISTANCE);
	m_data[S_TOP + 3] = ChunkMesh::VertexData(mx_y_mz * VIEW_DISTANCE);

	m_data[S_BOTTOM + 0] = ChunkMesh::VertexData(x_my_z * VIEW_DISTANCE);
	m_data[S_BOTTOM + 1] = ChunkMesh::VertexData(mx_my_z * VIEW_DISTANCE);
	m_data[S_BOTTOM + 2] = ChunkMesh::VertexData(mx_my_mz * VIEW_DISTANCE);
	m_data[S_BOTTOM + 3] = ChunkMesh::VertexData(x_my_mz * VIEW_DISTANCE);

	for (int i = 0; i < 24; i++)
	{
		switch (i % 4)
		{
		case 0:
			m_texData[i] = ChunkMesh::TextureData(br, i);
			break;
		case 1:
			m_texData[i] = ChunkMesh::TextureData(bl, i);
			break;
		case 2:
			m_texData[i] = ChunkMesh::TextureData(tl, i);
			break;
		case 3:
			m_texData[i] = ChunkMesh::TextureData(tr, i);
			break;
		}
	}

	m_texArray->AddTexture(TEXTURE_PATH "sky-left.png");
	m_texArray->AddTexture(TEXTURE_PATH "sky-center.png");
	m_texArray->AddTexture(TEXTURE_PATH "sky-right.png");
	m_texArray->AddTexture(TEXTURE_PATH "sky-back.png");
	m_texArray->AddTexture(TEXTURE_PATH "sky-top.png");
	m_texArray->AddTexture(TEXTURE_PATH "sky-bottom.png");

	m_texArray->Generate();

	m_mesh->SetMeshData(m_data, 24, m_texData);
}

void Skybox::Update(Vector3f &playerpos)
{
	if (playerpos != m_playerpos)
		m_playerpos = playerpos;
}
void Skybox::Render()
{
	glPushMatrix();
	glTranslatef(m_playerpos.x, m_playerpos.y, m_playerpos.z);
	m_texArray->Use();
	m_mesh->Render();
	glPopMatrix();
}