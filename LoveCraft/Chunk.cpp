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
		int maxVertexCount = ( CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z ) * 24;
		ChunkMesh :: VertexData * vd = new ChunkMesh :: VertexData [ maxVertexCount ];
		uint16* id = new uint16[3 * maxVertexCount / 2]; 
		int vertexCount = 0;
		int indexCount = 0;
		Array3d<bool> blocOptimized(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);
		blocOptimized.Reset(false);
		for ( int x = 0; x < CHUNK_SIZE_X ; ++x)
		{
			for ( int z = 0; z < CHUNK_SIZE_Z ; ++z)
			{
				for ( int y = 0; y < CHUNK_SIZE_Y ; ++y)
				{
					if( vertexCount > USHRT_MAX )
						break ;
					BlockType bt = GetBloc (x, y, z);
					if (y == CHUNK_SIZE_Y - 1 || bt == BTYPE_AIR || GetBloc(x, y + 1, z) != BTYPE_AIR)
					{
						blocOptimized.Set(x,y,z,true);
					}

					if (!blocOptimized.Get(x,y,z))	
					{
						OptimizeTopFaces(vd, vertexCount, id, indexCount, x, y, z, bt, blocOptimized);
					}
					if(bt != BTYPE_AIR )
					{
						AddBlockToMesh (vd , vertexCount , id, indexCount, bt , x, y, z);
					}
				}
			}
		}
		if( vertexCount > USHRT_MAX )
		{
			vertexCount = USHRT_MAX ;
			std :: cout << "[ Chunk :: Update ] Chunk data truncaned , too much vertices to have a 16 bit index " << std :: endl ;
		}
		m_chunkMesh . SetMeshData (vd , vertexCount, id, indexCount);
		delete [] vd;
		delete [] id;
	}
	m_isDirty = false ;
}

void Chunk::Render() const
{
	m_chunkMesh.Render();
}

void Chunk::OptimizeTopFaces(ChunkMesh::VertexData* vd, int& vertexCount, uint16* id, int& indexCount, int x, int y, int z, BlockType bt, Array3d<bool>& blocOptimized)
{
	// Itérateurs : i = x, j = z
	// Conditions d'arrêt : 
	//  - On est arrivé à la fin du chunk (CHUNK_SIZE - 1)
	//  - Le prochain bloc n'est pas du même type que @bt
	//  - Le bloc au dessus du prochain bloc n'est pas de l'air
	//  - Le prochain bloc est déja optimisé
	
	int endX = -1;
	int endY = -1;

	BlockType btest = GetBloc(3, 1, 3);

	// Parcourt le chunk en x
	for(int i = x; i < CHUNK_SIZE_X; ++i)
	{
		for(int j = z; j < CHUNK_SIZE_Z; ++j)
		{
			// Si une des conditions d'arrêt est vraie
			if ((j == CHUNK_SIZE_Z - 1) || (GetBloc(i, y, j + 1) != bt) || (GetBloc(i, y + 1, j + 1) != BTYPE_AIR) || blocOptimized.Get(i, y, j + 1))
			{
				// Trouve la plus petite position en Y qui répond à la condition d'arrêt
				if (endY == -1 || j < endY)
					endY = j;
				break;
			}
		}
		// Si une des conditions d'arrêt est vraie
		if ((i == CHUNK_SIZE_X - 1) || (GetBloc(i + 1, y, z) != bt) || (GetBloc(i + 1, y + 1, z) != BTYPE_AIR) || blocOptimized.Get(i + 1, y, z))
		{
			endX = i;
			break;
		}
	}

	// Update le tableau avec les nouveaux blocs optimisés
	for (int i = x; i <= endX; ++i)
	{
		for (int j = z; j <= endY; ++j)
		{
			blocOptimized.Set(i, y, j, true);
		}
	}

	// Crée le rectangle
	AddRectangleToMesh(vd, vertexCount, id, indexCount, x, y, z, endX - x + 1, endY - z + 1, bt);

}

void Chunk::AddRectangleToMesh(ChunkMesh::VertexData* vd, int& vertexCount, uint16* id, int& indexCount, int x, int y, int z, int w, int h, BlockType bt)
{
	// place le chunk a sa position dans le monde
	x += m_pos.x;
	y -= 2.f;
	z += m_pos.y;

	//determine les coords de la texture du type de bloc
	BlockInfo::TextureCoords coords = Info::Get().GetBlocInfo(bt)->GetTextureCoords();
	float x0 = coords.u;
	float y0 = coords.v;
	float x1 = coords.u + coords.w;
	float y1 = coords.v + coords.h;

	//top face
	int v = vertexCount;

	vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + 0.5f, z + h-1 + 0.5f, 1.f, 1.f, 1.f, x0, y0);		//0
	vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y + 0.5f, z + h-1 + 0.5f, 1.f, 1.f, 1.f, x1, y0);	//3
	vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f, 1.f, x1, y1);		//2
	vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f, 1.f, x0, y1);				//1

	id[indexCount++] = v;
	id[indexCount++] = v + 1;
	id[indexCount++] = v + 2;
	id[indexCount++] = v;
	id[indexCount++] = v + 2;
	id[indexCount++] = v + 3;
}

void Chunk::AddBlockToMesh(ChunkMesh::VertexData* vd, int& vertexCount, uint16* id, int& indexCount, BlockType bt, int x, int y, int z)
{
	// Optimisation faces cubes
	bool renderFrontFace = true;
	bool renderBackFace = true;
	bool renderTopFace = true;
	bool renderBottomFace = true;
	bool renderLeftFace = true;
	bool renderRightFace = true;

	// front
	if (z != 0 && GetBloc(x, y, z - 1) != BTYPE_AIR)
		renderFrontFace = false;

	// back
	if (z < CHUNK_SIZE_Z - 1 && GetBloc(x, y, z + 1) != BTYPE_AIR)
		renderBackFace = false;

	// top
	if (y < CHUNK_SIZE_Y - 1 && GetBloc(x, y + 1, z) != BTYPE_AIR)
		renderTopFace = false;

	// bottom
	if (y != 0 && GetBloc(x, y - 1, z) != BTYPE_AIR)
		renderBottomFace = false;

	// left
	if (x != 0 && GetBloc(x - 1, y, z) != BTYPE_AIR)
		renderLeftFace = false;

	// right
	if (x < CHUNK_SIZE_X - 1 && GetBloc(x + 1, y, z) != BTYPE_AIR)
		renderRightFace = false;

	// plancher
	if (y == 0)
		renderBottomFace = renderLeftFace = renderRightFace = renderFrontFace = renderBackFace = false;

	//determine les coords de la texture du type de bloc
	BlockInfo::TextureCoords coords = Info::Get().GetBlocInfo(bt)->GetTextureCoords();
	float x0 = coords.u;
	float y0 = coords.v;
	float x1 = coords.u + coords.w;
	float y1 = coords.v + coords.h;

	// place le chunk a sa position dans le monde
	x += m_pos.x;
	y -= 2.f;
	z += m_pos.y;

	int v;

	//front face
	if (renderFrontFace)
	{
		v = vertexCount;

		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + 0.5f, z + 0.5f, 0.9f, 0.9f, 0.9f, x0, y0);		//0
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z + 0.5f, 0.9f, 0.9f, 0.9f, x0, y1);		//1
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y - 0.5f, z + 0.5f, 0.9f, 0.9f, 0.9f, x1, y1);		//2
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y + 0.5f, z + 0.5f, 0.9f, 0.9f, 0.9f, x1, y0);		//3

		id[indexCount++] = v;
		id[indexCount++] = v + 1;
		id[indexCount++] = v + 2;
		id[indexCount++] = v;
		id[indexCount++] = v + 2;
		id[indexCount++] = v + 3;
	}

	//back face
	if (renderBackFace)
	{
		v = vertexCount;

		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f, x0, y0);		//0
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y + 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f, x1, y0);		//1
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y - 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f, x1, y1);		//2
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f, x0, y1);		//3

		id[indexCount++] = v;
		id[indexCount++] = v + 1;
		id[indexCount++] = v + 2;
		id[indexCount++] = v;
		id[indexCount++] = v + 2;
		id[indexCount++] = v + 3;
	}

	/*if (renderTopFace)
	{

	v = vertexCount;

	vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + 0.5f, z + 0.5f, 1.f, 1.f, 1.f, x0, y0);		//0
	vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y + 0.5f, z + 0.5f, 1.f, 1.f, 1.f, x1, y0);		//3
	vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f, 1.f, x1, y1);		//2
	vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f, 1.f, x0, y1);		//1

	id[indexCount++] = v;
	id[indexCount++] = v + 1;
	id[indexCount++] = v + 2;
	id[indexCount++] = v;
	id[indexCount++] = v + 2;
	id[indexCount++] = v + 3;
	}*/
	//bot face
	if (renderBottomFace)
	{
		v = vertexCount;

		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z + 0.5f, 0.6f, 0.6f, 0.6f, x0, y0);		//0
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f, x0, y1);		//3
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y - 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f, x1, y1);		//2
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y - 0.5f, z + 0.5f, 0.6f, 0.6f, 0.6f, x1, y0);		//1

		id[indexCount++] = v;
		id[indexCount++] = v + 1;
		id[indexCount++] = v + 2;
		id[indexCount++] = v;
		id[indexCount++] = v + 2;
		id[indexCount++] = v + 3;
	}

	//left face
	if (renderLeftFace)
	{
		v = vertexCount;

		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z + 0.5f, 0.8f, 0.8f, 0.8f, x0, y0);		//0
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + 0.5f, z + 0.5f, 0.8f, 0.8f, 0.8f, x0, y1);		//3
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + 0.5f, z - 0.5f, 0.8f, 0.8f, 0.8f, x1, y1);		//2
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z - 0.5f, 0.8f, 0.8f, 0.8f, x1, y0);		//1

		id[indexCount++] = v;
		id[indexCount++] = v + 1;
		id[indexCount++] = v + 2;
		id[indexCount++] = v;
		id[indexCount++] = v + 2;
		id[indexCount++] = v + 3;
	}

	//right face
	if (renderRightFace)
	{
		v = vertexCount;

		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y - 0.5f, z + 0.5f, 0.6f, 0.6f, 0.6f, x0, y0);		//0
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y - 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f, x1, y0);		//3
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y + 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f, x1, y1);		//2
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y + 0.5f, z + 0.5f, 0.6f, 0.6f, 0.6f, x0, y1);		//1

		id[indexCount++] = v;
		id[indexCount++] = v + 1;
		id[indexCount++] = v + 2;
		id[indexCount++] = v;
		id[indexCount++] = v + 2;
		id[indexCount++] = v + 3;
	}
}

Vector2i Chunk::GetPosition() const
{
	return m_pos;
}

void Chunk::SetPosition( Vector2i pos )
{
	m_pos = pos;
}