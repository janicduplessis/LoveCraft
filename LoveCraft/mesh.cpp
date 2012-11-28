#include "mesh.h"
#include <cassert>
#include <climits>
#include <iostream>

Mesh::Mesh() : m_isValid(false)
{
}

Mesh::~Mesh()
{
    if(m_isValid)
    {
        glDeleteBuffers(1, &m_vertexVboId);
        glDeleteBuffers(1, &m_indexVboId);
    }
}

bool Mesh::IsValid() const
{
    return m_isValid;
}

void Mesh::SetMeshData(VertexData* vd, int vertexCount, uint16* indexData, int indexCount){
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

	if (indexCount == 0 && indexData == 0)
	{
		idx = new uint16[vertexCount];
		for(int i = 0; i < vertexCount; ++i)
			idx[i] = i;
		m_indicesCount = vertexCount;
	}
	else
	{
		m_indicesCount = indexCount;
	}
	
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);

	if (indexCount == 0 && indexData == 0) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * vertexCount, idx, GL_STATIC_DRAW);
		delete [] idx;
	}
	else
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * indexCount, indexData, GL_STATIC_DRAW);
	}

    m_isValid = true;
}

void Mesh::Render(bool wireFrame) const
{
    if(IsValid())
    {
        //glClientActiveTexture(GL_TEXTURE0);
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

int Mesh::Count() const
{
    return m_indicesCount;
}

