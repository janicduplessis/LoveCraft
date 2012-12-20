#include "texturearray.h"


TextureArray::TextureArray(int textureSize ) : m_textureSize(textureSize)
{
	
}

void TextureArray::Use()
{

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
}

void TextureArray::Init()
{
	int slotSize = m_textureSize * m_textureSize * 4;
	int size = m_textureList.size() * slotSize; // height * width * 4 ( RGBA)
	m_data = new unsigned char[size];

	memset(m_data, 0x11, size); // setter a blanc...

	// Initialize Devil only once:
	static bool alreadyInitialized = false;
	if(!alreadyInitialized)
	{
		ilInit();
		iluInit();
		alreadyInitialized = true;
	}

	int count = 0;
	for(TextureList::iterator it = m_textureList.begin(); it != m_textureList.end(); ++it)
	{
		ILuint texid = it->second.texId;
		if(texid == (ILuint)-1)
		{
			std::cout << "Loading " << it->first << " (id=" << it->second.texIdx << ")..." << std::endl;
			ilGenImages(1, &texid);
			ilBindImage(texid);

			ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
			ilEnable(IL_ORIGIN_SET);

			if (!ilLoadImage((const ILstring)it->first.c_str()))
				return;

			if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
				return;

			iluScale(m_textureSize, m_textureSize, 1);

			it->second.texId = texid;

			memcpy(m_data + (count * slotSize), ilGetData(), slotSize);

			ilDeleteImages(1, &texid);
		}
	}

	int sliceCount = m_textureList.size();

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_GENERATE_MIPMAP, GL_TRUE);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glTexImage3D(GL_TEXTURE_2D_ARRAY,0,GL_RGBA,m_textureSize,m_textureSize,sliceCount,0,GL_RGBA,GL_UNSIGNED_BYTE,m_data);
}

TextureArray::TextureIndex TextureArray::AddTexture(const std::string& fname)
{
	TextureList::iterator it = m_textureList.find(fname);

	if(it != m_textureList.end())
		return it->second.texIdx;

	TextureIndex id = m_currentTextureIndex++;
	m_textureList.insert(std::make_pair(fname, TextureInfo((ILuint)-1, id)));

	return id;
}

TextureArray::~TextureArray()
{
	delete [] m_data;
}
