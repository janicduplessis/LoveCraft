#include "chunkmesh.h"
#include <cassert>
#include "util/tool.h"


ChunkMesh::ChunkMesh()
{}

ChunkMesh::~ChunkMesh()
{
}

bool ChunkMesh::IsValid() const
{
	return Mesh::IsValid();
}

void ChunkMesh::SetMeshData(VertexData* vertexData, int vertexCount)
{
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

	if(!m_isValid)
	{
		glGenBuffers(1, &m_vertexVboId);
		glGenBuffers(1, &m_indexVboId);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, vertexData, GL_STATIC_DRAW);

	
	m_indicesCount = indexCount;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * m_indicesCount, indexData, GL_STATIC_DRAW);

	m_isValid = true;
}


void ChunkMesh::Render(bool wireFrame) const
{
	if(IsValid())
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		CHECK_GL_ERROR();
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid*)24);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid*)36);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
		CHECK_GL_ERROR();
		glDrawElements(wireFrame ? GL_LINES : GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_SHORT, (char*)0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		CHECK_GL_ERROR();
	}
}

int ChunkMesh::Count() const
{
	return Mesh::Count();
}
