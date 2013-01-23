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

	/**
	 * Bloc avec plusieurs texture
	 * 0 = top
	 * 1 = sides
	 * 2 = bottom
	 */
	void SetTextureIndex(int index, TextureArray::TextureIndex coords);
	TextureArray::TextureIndex GetTextureIndex(int index) const;

    void Afficher() const;

private:
    BlockType m_type;
    std::string m_nom;
    int m_durabilite;
	TextureArray::TextureIndex m_textureIndex[3];
	bool m_isSolid;
};

#endif // BLOCKINFO_H__
