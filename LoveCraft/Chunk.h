#ifndef CHUNK_H__
#define CHUNK_H__
#include "array3d.h"
#include "define.h"

class Chunk
{
    public:
    Chunk();
    ~Chunk();

    void RemoveBloc(int x, int y, int z);
    void SetBloc(int x, int y, int z, BLOCK_TYPE type);
    BLOCK_TYPE GetBloc(int x, int y, int z);

private:
	Array3d<BLOCK_TYPE> m_blocks;
};

#endif // CHUNK_H__
