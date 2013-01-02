#ifndef TEXTURE_H__
#define TEXTURE_H__

#include "define.h"

#include <IL/il.h>
#include <string>

#include "util/vector2.h"

class Texture
{
public:
    Texture(const std::string& filename = "");
    ~Texture();

    bool Load(const std::string& filename);
    bool IsValid() const;
    void Bind() const;

	float GetWidth();
	float GetHeight();

private:
    GLuint  m_textureId;
    bool    m_isValid;
	Vector2i m_dimensions;
};

#endif // TEXTURE_H__
