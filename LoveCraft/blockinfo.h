#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

#include <string>
#include "define.h"

class BlockInfo
{
public:
	struct TextureCoords
	{
		float u;
		float v;
		float h;
		float w;
	};

public:
    BlockInfo(BlockType type, const std::string& nom);
    ~BlockInfo();

    BlockType GetType() const;

    void SetDurabilite(int durabilite);
    int GetDurabilite() const;

	void SetTextureCoords(TextureCoords coords);
	TextureCoords GetTextureCoords() const;

    void Afficher() const;

private:
    BlockType m_type;
    std::string m_nom;
    int m_durabilite;
	TextureCoords m_textureCoords;
};

#endif // BLOCKINFO_H__
