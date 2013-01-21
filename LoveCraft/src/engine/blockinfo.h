#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

#include "define.h"

#include "gl/texturearray.h"
#include <string>

class BlockInfo
{
public:
    BlockInfo(BlockType type, const std::string& nom, bool isSolid);
    ~BlockInfo();

    BlockType GetType() const;

    void SetDurabilite(int durabilite);
    int GetDurabilite() const;

	bool IsSolid() const;

	void SetTextureIndex(TextureArray::TextureIndex index);
	TextureArray::TextureIndex GetTextureIndex() const;

    void Afficher() const;

private:
    BlockType m_type;
    std::string m_nom;
    int m_durabilite;
	TextureArray::TextureIndex m_textureIndex;
	bool m_isSolid;
};

#endif // BLOCKINFO_H__
