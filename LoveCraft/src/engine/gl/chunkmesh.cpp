#include "chunkmesh.h"
#include <cassert>
#include "util/tool.h"


ChunkMesh::ChunkMesh(Shader* shader) : m_shader(shader)
{

}

ChunkMesh::~ChunkMesh()
{
	glDeleteBuffers(1, &m_textureVboId);
}

bool ChunkMesh::IsValid() const
{
	return Mesh::IsValid();
}

void ChunkMesh::SetMeshData(VertexData* vd, int vertexCount, TextureData* td)
{
	glewInit();

	assert(vertexCount <= USHRT_MAX);
	if(vertexCount == 0)
		return;

	int indexCount = 0;
	uint16* indexData = new uint16[3 * vertexCount / 2];

	int faceCount = vertexCount / 4.f;
	// Genere les index
	for (int i = 0; i < faceCount; ++i)
	{
		int v = i * 4;
		indexData[indexCount++] = v;
		indexData[indexCount++] = v + 1;
		indexData[indexCount++] = v + 2;
		indexData[indexCount++] = v;
		indexData[indexCount++] = v + 2;
		indexData[indexCount++] = v + 3;
	}
	CHECK_GL_ERROR();

	if(!m_isValid)
	{
		glGenBuffers(1, &m_vertexVboId);
		glGenBuffers(1, &m_indexVboId);
		glGenBuffers(1, &m_textureVboId);
	}

	m_textureLoc = glGetAttribLocation(m_shader->m_program, "Texture");

	CHECK_GL_ERROR();

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, vd, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_textureVboId);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(TextureData), td, GL_STATIC_DRAW);

	glEnableVertexAttribArray(m_textureLoc);
	glVertexAttribPointer(m_textureLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	m_indicesCount = indexCount;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * m_indicesCount, indexData, GL_STATIC_DRAW);

	CHECK_GL_ERROR();

	m_isValid = true;
}


void ChunkMesh::Render(bool wireFrame) const
{
	if(IsValid())
	{
		//glClientActiveTexture(GL_TEXTURE0);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(VertexData), (char*)0);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, sizeof(VertexData), (char*)12);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
		glDrawElements(wireFrame ? GL_LINES : GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_SHORT, (char*)0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
}

int ChunkMesh::Count() const
{
	return Mesh::Count();
}
