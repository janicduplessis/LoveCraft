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

void Mesh::SetMeshData(VertexData* vd, int vertexCount){
	glewInit();

    assert(vertexCount <= USHRT_MAX);
    if(vertexCount == 0)
        return;

    if(!m_isValid)
    {
        glGenBuffers(1, &m_vertexVboId);
        glGenBuffers(1, &m_indexVboId);
    }

    m_vertexCount = vertexCount;

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, vd, GL_STATIC_DRAW);

    // Pour le moment, generer le index array pour inclure tout les vertex, sans 
    // optimisation pour reduire le nombre de vertex envoyes a la carte
    // Idealement cet array devrait etre utiliser pour reutiliser les vertex et ainsi
    // sauver du temps en envoyant moins de donnees a la carte (il devrait etre construit
    // en meme temps que le buffer vd est rempli..)
    
	int nbIndex = (int)(vertexCount / 4.f * 6);
    uint16* idx = new uint16[nbIndex];
	uint16 j = 0;
    for(uint16 i = 0; i < nbIndex; ++i)
	{
		switch (nbIndex % 6)
		{
		case 0:
			idx[i] = i;
		default:
			break;
		}
        idx[j++] = i;
		idx[j++] = i + 1;
		idx[j++] = i + 2;
		idx[j++] = i;
		idx[j++] = i + 2;
		idx[j++] = i + 3;
		if (!(j < nbIndex + 1))
			j = j;
	}

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * nbIndex, idx, GL_STATIC_DRAW);
    delete [] idx;

    m_isValid = true;
}

void Mesh::Render(bool wireFrame) const
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
        //glDrawElements(wireFrame ? GL_LINES : GL_QUADS, m_vertexCount, GL_UNSIGNED_SHORT, (char*)0);
        glDrawElements(GL_TRIANGLES, m_vertexCount, GL_UNSIGNED_SHORT, (char*)0);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}

int Mesh::Count() const
{
    return m_vertexCount;
}

