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

void Mesh::SetMeshData(VertexData* vd, int vertexCount, uint16* indexData, int indexCount) {}

void Mesh::Render(bool wireFrame) const {}

int Mesh::Count() const
{
    return m_indicesCount;
}

