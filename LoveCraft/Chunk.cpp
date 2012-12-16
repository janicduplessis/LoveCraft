#include "chunk.h"

Chunk::Chunk() : m_pos(0), m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z)
{
	m_blocks.Reset(BTYPE_AIR);
}

Chunk::~Chunk()
{
}

void Chunk::RemoveBloc(uint32 x, uint32 y, uint32 z)
{
	m_blocks.Set(x, y, z, BTYPE_AIR);
	m_isDirty = true;
}

void Chunk::SetBloc(uint32 x, uint32 y, uint32 z, BlockType type)
{
	m_blocks.Set(x, y, z, type);
	m_isDirty = true;
}

BlockType Chunk::GetBloc(uint32 x, uint32 y, uint32 z)
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
		int maxVertexCount = ( CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z ) * 12;
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
						//ajoute le block a la position relative au chunk + la position du chunk dans le monde
						AddBlockToMesh (vd , count , bt , x + m_pos.x, y - 2, z + m_pos.y);
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
	//determine les coords de la texture du type de bloc
	BlockInfo::TextureCoords coords = Info::Get().GetBlocInfo(bt)->GetTextureCoords();
	float x0 = coords.u;
	float y0 = coords.v;
	float x1 = coords.u + coords.w;
	float y1 = coords.v + coords.h;
	// cube 12 vertex!!!
	// vertices pour utiliser avec index
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + 0.5f, z + 0.5f, 0.f, 0.f, 1.f, x1, y1);		//0
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + -0.5f, z + 0.5f, 1.f, 0.f, 0.f, x1, y0);		//1
	vd[ count ++] = ChunkMesh::VertexData (x + 0.5f, y + -0.5f, z + 0.5f, 1.f, 1.f, 0.f, x1, y1);		//2
	vd[ count ++] = ChunkMesh::VertexData (x + 0.5f, y + 0.5f, z + 0.5f, 0.f, 1.f, 0.f, x1, y0);		//3
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + 0.5f, z + -0.5f, 0.f, 0.f, 0.f, x0, y1);		//4
	vd[ count ++] = ChunkMesh::VertexData (x + 0.5f, y + 0.5f, z + -0.5f, 1.f, 0.f, 0.f, x0, y0);		//5
	vd[ count ++] = ChunkMesh::VertexData (x + 0.5f, y + -0.5f, z + -0.5f, 1.f, 1.f, 0.f, x0, y1);		//6
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + -0.5f, z + -0.5f, 0.f, 1.f, 0.f, x0, y0);		//7
	// vertices additionnels pour uv
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + 0.5f, z + 0.5f, 0.f, 0.f, 1.f, x0, y0);		//8
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + -0.5f, z + 0.5f, 1.f, 0.f, 0.f, x0, y1);		//9
	vd[ count ++] = ChunkMesh::VertexData (x + -0.5f, y + 0.5f, z + -0.5f, 0.f, 0.f, 0.f, x1, y0);		//10
	vd[ count ++] = ChunkMesh::VertexData (x + 0.5f, y + 0.5f, z + -0.5f, 1.f, 0.f, 0.f, x1, y1);		//11
}

Vector2i Chunk::GetPosition() const
{
	return m_pos;
}

void Chunk::SetPosition( Vector2i pos )
{
	m_pos = pos;
}

