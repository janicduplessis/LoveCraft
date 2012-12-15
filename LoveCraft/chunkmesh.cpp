#include "chunkmesh.h"
#include <cassert>


ChunkMesh::ChunkMesh()
{

}

ChunkMesh::~ChunkMesh()
{

}

bool ChunkMesh::IsValid() const
{
	return Mesh::IsValid();
}

void ChunkMesh::SetMeshData(VertexData* vd, int vertexCount, uint16* indexData, int indexCount)
{
	glewInit();

	assert(vertexCount <= USHRT_MAX);
	if(vertexCount == 0)
		return;

	if(!m_isValid)
	{
		glGenBuffers(1, &m_vertexVboId);
		glGenBuffers(1, &m_indexVboId);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, vd, GL_STATIC_DRAW);

	// Pour le moment, generer le index array pour inclure tout les vertex, sans 
	// optimisation pour reduire le nombre de vertex envoyes a la carte
	// Idealement cet array devrait etre utiliser pour reutiliser les vertex et ainsi
	// sauver du temps en envoyant moins de donnees a la carte (il devrait etre construit
	// en meme temps que le buffer vd est rempli..)

	uint16* idx;

	m_indicesCount = (int)(36 * vertexCount / 12.f);

	idx = new uint16[m_indicesCount];
	int j = 0;
	for(uint16 i = 0; i < vertexCount / 12; ++i)
	{
		//front face
		idx[j++] = 12*i + 8;
		idx[j++] = 12*i + 9;
		idx[j++] = 12*i + 2;
		idx[j++] = 12*i + 8;
		idx[j++] = 12*i + 2;
		idx[j++] = 12*i + 3;

		//back face
		idx[j++] = 12*i + 10;
		idx[j++] = 12*i + 11;
		idx[j++] = 12*i + 6;
		idx[j++] = 12*i + 10;
		idx[j++] = 12*i + 6;
		idx[j++] = 12*i + 7;

		//top face
		idx[j++] = 12*i + 4;
		idx[j++] = 12*i;
		idx[j++] = 12*i + 3;
		idx[j++] = 12*i + 4;
		idx[j++] = 12*i + 3;
		idx[j++] = 12*i + 5;

		//bot face
		idx[j++] = 12*i + 1;
		idx[j++] = 12*i + 7;
		idx[j++] = 12*i + 2;
		idx[j++] = 12*i + 2;
		idx[j++] = 12*i + 7;
		idx[j++] = 12*i + 6;

		//left face
		idx[j++] = 12*i + 1;
		idx[j++] = 12*i + 0;
		idx[j++] = 12*i + 7;
		idx[j++] = 12*i + 0;
		idx[j++] = 12*i + 4;
		idx[j++] = 12*i + 7;

		//right face
		idx[j++] = 12*i + 2;
		idx[j++] = 12*i + 6;
		idx[j++] = 12*i + 3;
		idx[j++] = 12*i + 3;
		idx[j++] = 12*i + 6;
		idx[j++] = 12*i + 5;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * m_indicesCount, idx, GL_STATIC_DRAW);

	delete [] idx;

	m_isValid = true;
}


void ChunkMesh::Render(bool wireFrame) const
{
	if(IsValid())
	{
		glClientActiveTexture(GL_TEXTURE0);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(VertexData), (char*)0);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, sizeof(VertexData), (char*)12);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData), (char*)24);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(VertexData), (char*)32);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
		glDrawElements(wireFrame ? GL_LINES : GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_SHORT, (char*)0);

		//glDrawRangeElements(GL_TRIANGLES, 0, m_indicesCount, m_indicesCount, GL_UNSIGNED_SHORT, (char*)0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
}

int ChunkMesh::Count() const
{
	return Mesh::Count();
}
