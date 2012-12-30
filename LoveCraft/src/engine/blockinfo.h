#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

#include "define.h"

#include "gl/texturearray.h"
#include <string>

#include "son.h"

class BlockInfo
{
public:
    BlockInfo(BlockType type, const std::string& nom);
    ~BlockInfo();

    BlockType GetType() const;
	Son::Foots Getsound() const;

    void SetDurabilite(int durabilite);
    int GetDurabilite() const;

	void SetTextureIndex(TextureArray::TextureIndex index);
	TextureArray::TextureIndex GetTextureIndex() const;

    void Afficher() const;

private:
    BlockType m_type;
    std::string m_nom;
    int m_durabilite;
	TextureArray::TextureIndex m_textureIndex;
};

#endif // BLOCKINFO_H__
