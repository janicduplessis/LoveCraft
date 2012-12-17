#include "chunk.h"

Chunk::Chunk() : m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z)
{
	m_blocks.Reset(BTYPE_AIR);

	for ( int x = 0; x < CHUNK_SIZE_X ; ++x)
	{
		for ( int z = 0; z < CHUNK_SIZE_Z ; ++z)
		{
			for ( int y = 0; y < 32; ++y)
			{
				if(x % 2 == 0 && y % 2 == 0 && z % 2 == 0)
					SetBloc(x, y, z, BTYPE_DIRT );
			}
		}
	}
}

Chunk::~Chunk()
{
}

void Chunk::RemoveBloc(int x, int y, int z)
{
	m_blocks.Set(x, y, z, BTYPE_AIR);
	m_isDirty = true;
}

void Chunk::SetBloc(int x, int y, int z, BlockType type)
{
	m_blocks.Set(x, y, z, type);
	m_isDirty = true;
}

BlockType Chunk::GetBloc(int x, int y, int z)
{
	return m_blocks.Get(x, y, z);
}

bool Chunk::IsDirty() const
{
	return m_isDirty;
}

void Chunk::Update()
{
	// Update mesh
	if( m_isDirty )
	{
		int maxVertexCount = ( CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z ) * (6 * 4);
		ChunkMesh :: VertexData * vd = new ChunkMesh :: VertexData [ maxVertexCount ];
		int count = 0;
		for ( int x = 0; x < CHUNK_SIZE_X ; ++x)
		{
			for ( int z = 0; z < CHUNK_SIZE_Z ; ++z)
			{
				for ( int y = 0; y < CHUNK_SIZE_Y ; ++y)
				{
					if( count > USHRT_MAX )
						break ;
					BlockType bt = GetBloc (x, y, z);
					if(bt != BTYPE_AIR )
					{
						AddBlockToMesh (vd , count , bt , x, y, z);
					}
				}
			}
		}
		if( count > USHRT_MAX )
		{
			count = USHRT_MAX ;
			std :: cout << "[ Chunk :: Update ] Chunk data truncaned , too much vertices to have a 16 bit index " << std :: endl ;
		}
		m_chunkMesh . SetMeshData (vd , count );
		delete [] vd;
	}
	m_isDirty = false ;
}

void Chunk::Render() const
{
	m_chunkMesh.Render();
}

void Chunk::AddBlockToMesh(ChunkMesh::VertexData* vd, int& count, BlockType bt, int x, int y, int z)
{
	// cube 12 vertex!!!
	// vertices pour utiliser avec index
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + 0.5f, z + 0.5f, 0.f, 0.f, 1.f, 1.f, 1.f);		//0
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + -0.5f, z + 0.5f, 1.f, 0.f, 0.f, 1.f, 0.f);	//1
	vd[ count ++] = ChunkMesh::VertexData (x + 0.5f, y + -0.5f, z + 0.5f, 1.f, 1.f, 0.f, 1.f, 1.f);		//2
	vd[ count ++] = ChunkMesh::VertexData (x + 0.5f, y + 0.5f, z + 0.5f, 0.f, 1.f, 0.f, 1.f, 0.f);		//3
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + 0.5f, z + -0.5f, 0.f, 0.f, 0.f, 0.f, 1.f);	//4
	vd[ count ++] = ChunkMesh::VertexData (x + 0.5f, y + 0.5f, z + -0.5f, 1.f, 0.f, 0.f, 0.f, 0.f);		//5
	vd[ count ++] = ChunkMesh::VertexData (x + 0.5f, y + -0.5f, z + -0.5f, 1.f, 1.f, 0.f, 0.f, 1.f);	//6
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + -0.5f, z + -0.5f, 0.f, 1.f, 0.f, 0.f, 0.f);	//7
	// vertices additionnels pour uv
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + 0.5f, z + 0.5f, 0.f, 0.f, 1.f, 0.f, 0.f);		//8
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + -0.5f, z + 0.5f, 1.f, 0.f, 0.f, 0.f, 1.f);	//9
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + 0.5f, z + -0.5f, 0.f, 0.f, 0.f, 1.f, 0.f);	//10
	vd[ count ++] = ChunkMesh::VertexData (x + 0.5f, y + 0.5f, z + -0.5f, 1.f, 0.f, 0.f, 1.f, 1.f);		//11
}