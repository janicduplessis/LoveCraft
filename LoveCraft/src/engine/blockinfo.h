#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

#include "define.h"

#include "gl/textures/texturearray.h"
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
	void SetColorTextureIndex(int index, TextureArray::TextureIndex texIndex);
	void SetNormalTextureIndex(int index, TextureArray::TextureIndex texIndex);
	TextureArray::TextureIndex GetColorTextureIndex(int index) const;
	TextureArray::TextureIndex GetNormalTextureIndex(int index) const;

    void Afficher() const;
	
private:
    BlockType m_type;
    std::string m_nom;
    int m_durabilite;
	TextureArray::TextureIndex m_colorTextureIndex[3];
	TextureArray::TextureIndex m_normalTextureIndex[3];
	bool m_isSolid;
};

#endif // BLOCKINFO_H__
