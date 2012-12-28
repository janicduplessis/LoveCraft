#ifndef MESH_H__
#define MESH_H__
#include "define.h"

class Mesh
{
public:
    Mesh();
    virtual ~Mesh();
    
    virtual bool IsValid() const;
    virtual void Render(bool wireFrame = false) const = 0;

    virtual int Count() const;



protected:
    bool m_isValid;
    int m_indicesCount;
    GLuint m_vertexVboId;
    GLuint m_indexVboId;
};

#endif // MESH_H__
