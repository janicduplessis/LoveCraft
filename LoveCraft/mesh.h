#ifndef MESH_H__
#define MESH_H__
#include "define.h"

class Mesh
{
public:
    // Structure représentant toutes les informations de chacuns des vertex
    // S'assurer que le size de cette struct reste un multiple de 32
    // octet pour un maximum de performance
    // Au besoin, ajouter du padding
    struct VertexData
    {
        float x, y, z;
        float r, g, b;
        float u, v;
		float n1, n2, n3;

        VertexData() {}
        VertexData(float _x, float _y, float _z, float _r, float _g, float _b, float _u, float _v) : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), u(_u), v(_v) {}
    };

public:
    Mesh();
    virtual ~Mesh();
    
    virtual bool IsValid() const;
    virtual void SetMeshData(VertexData* vd, int vertexCount, uint16* indexData = 0, int indexCount = 0) = 0;
    virtual void Render(bool wireFrame = false) const = 0;

    virtual int Count() const;



protected:
    bool m_isValid;
    int m_indicesCount;
    GLuint m_vertexVboId;
    GLuint m_indexVboId;
};

#endif // MESH_H__
