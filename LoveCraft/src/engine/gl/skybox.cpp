#include "skybox.h"


Skybox::Skybox() : m_playerpos(Vector3f()), m_mesh(Info::Get().GetCubeShader()), m_texArray(128)
{
}


Skybox::~Skybox()
{

}

void Skybox::Init()
{
	ChunkMesh::VertexData* data = new ChunkMesh::VertexData[24];
	ChunkMesh::TextureData* texData = new ChunkMesh::TextureData[24];
	
	Vector3f mx_my_mz(-1, -1, -1);
	Vector3f x_my_mz(1, -1, -1);
	Vector3f mx_y_mz(-1, 1, -1);
	Vector3f x_y_mz(1, 1, -1);
	Vector3f mx_my_z(-1, -1, 1);
	Vector3f x_my_z(1, -1, 1);
	Vector3f mx_y_z(-1, 1, 1);
	Vector3f x_y_z(1, 1, 1);

	Vector2i tl(-1, 1);
	Vector2i tr(1, 1);
	Vector2i bl(-1, -1);
	Vector2i br(1, -1);

	data[0] = ChunkMesh::VertexData(mx_my_mz * VIEW_DISTANCE);
	data[1] = ChunkMesh::VertexData(mx_my_z * VIEW_DISTANCE);
	data[2] = ChunkMesh::VertexData(mx_y_z * VIEW_DISTANCE);
	data[3] = ChunkMesh::VertexData(mx_y_mz * VIEW_DISTANCE);
	texData[0] = ChunkMesh::TextureData(bl, 0);
	texData[1] = ChunkMesh::TextureData(br, 0);
	texData[2] = ChunkMesh::TextureData(tr, 0);
	texData[3] = ChunkMesh::TextureData(tl, 0);

	data[4] = ChunkMesh::VertexData(mx_my_z * VIEW_DISTANCE);
	data[5] = ChunkMesh::VertexData(x_my_z * VIEW_DISTANCE);
	data[6] = ChunkMesh::VertexData(x_y_z * VIEW_DISTANCE);
	data[7] = ChunkMesh::VertexData(mx_y_z * VIEW_DISTANCE);
	texData[4] = ChunkMesh::TextureData(bl, 1);
	texData[5] = ChunkMesh::TextureData(br, 1);
	texData[6] = ChunkMesh::TextureData(tr, 1);
	texData[7] = ChunkMesh::TextureData(tl, 1);

	data[8] = ChunkMesh::VertexData(x_my_z * VIEW_DISTANCE);
	data[9] = ChunkMesh::VertexData(x_my_mz * VIEW_DISTANCE);
	data[10] = ChunkMesh::VertexData(x_y_mz * VIEW_DISTANCE);
	data[11] = ChunkMesh::VertexData(x_y_z * VIEW_DISTANCE);
	texData[8] = ChunkMesh::TextureData(bl, 2);
	texData[9] = ChunkMesh::TextureData(br, 2);
	texData[10] = ChunkMesh::TextureData(tr, 2);
	texData[11] = ChunkMesh::TextureData(tl, 2);

	data[12] = ChunkMesh::VertexData(x_my_mz * VIEW_DISTANCE);
	data[13] = ChunkMesh::VertexData(mx_my_mz * VIEW_DISTANCE);
	data[14] = ChunkMesh::VertexData(mx_y_mz * VIEW_DISTANCE);
	data[15] = ChunkMesh::VertexData(x_y_mz * VIEW_DISTANCE);
	texData[12] = ChunkMesh::TextureData(bl, 3);
	texData[13] = ChunkMesh::TextureData(br, 3);
	texData[14] = ChunkMesh::TextureData(tr, 3);
	texData[15] = ChunkMesh::TextureData(tl, 3);

	data[16] = ChunkMesh::VertexData(mx_y_z * VIEW_DISTANCE);
	data[17] = ChunkMesh::VertexData(x_y_z * VIEW_DISTANCE);
	data[18] = ChunkMesh::VertexData(x_y_mz * VIEW_DISTANCE);
	data[19] = ChunkMesh::VertexData(mx_y_mz * VIEW_DISTANCE);
	texData[16] = ChunkMesh::TextureData(bl, 4);
	texData[17] = ChunkMesh::TextureData(br, 4);
	texData[18] = ChunkMesh::TextureData(tr, 4);
	texData[19] = ChunkMesh::TextureData(tl, 4);

	data[20] = ChunkMesh::VertexData(x_my_z * VIEW_DISTANCE);
	data[21] = ChunkMesh::VertexData(mx_my_z * VIEW_DISTANCE);
	data[22] = ChunkMesh::VertexData(mx_my_mz * VIEW_DISTANCE);
	data[23] = ChunkMesh::VertexData(x_my_mz * VIEW_DISTANCE);
	texData[20] = ChunkMesh::TextureData(bl, 5);
	texData[21] = ChunkMesh::TextureData(br, 5);
	texData[22] = ChunkMesh::TextureData(tr, 5);
	texData[23] = ChunkMesh::TextureData(tl, 5);

	m_texArray.AddTexture(TEXTURE_PATH "sky-left.png");
	m_texArray.AddTexture(TEXTURE_PATH "sky-center.png");
	m_texArray.AddTexture(TEXTURE_PATH "sky-right.png");
	m_texArray.AddTexture(TEXTURE_PATH "sky-back.png");
	m_texArray.AddTexture(TEXTURE_PATH "sky-top.png");
	m_texArray.AddTexture(TEXTURE_PATH "sky-bottom.png");

	m_texArray.Generate();

	m_mesh.SetMeshData(data, 24, texData);

	delete [] data;
	delete [] texData;
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
	m_texArray.Use();
	m_mesh.Render();
	glPopMatrix();
}