#include "chunk.h"

Chunk::Chunk(Shader* shader) : m_pos(0), m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), m_chunkMesh(shader)
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
		Chunk* leftChunk = 0;
		Chunk* rightChunk = 0;
		Chunk* frontChunk = 0;
		Chunk* backChunk = 0;

		Array2d<Chunk>* chunks = Info::Get().GetChunkArray();

		if (m_pos.x != 0)
			leftChunk = &chunks->Get(m_pos.x - 1, m_pos.y);

		if(m_pos.x != VIEW_DISTANCE / CHUNK_SIZE_X * 2 - 1)
			rightChunk = &chunks->Get(m_pos.x + 1, m_pos.y);

		if (m_pos.y != 0)
			frontChunk = &chunks->Get(m_pos.x, m_pos.y - 1);

		if(m_pos.y != VIEW_DISTANCE / CHUNK_SIZE_Z * 2 - 1)
			backChunk = &chunks->Get(m_pos.x, m_pos.y + 1);

		int maxVertexCount = ( CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z ) * 24;
		ChunkMesh :: VertexData * vd = new ChunkMesh :: VertexData [ maxVertexCount ];
		uint16* id = new uint16[3 * maxVertexCount / 2]; 
		ChunkMesh::TextureData* td = new ChunkMesh::TextureData[ maxVertexCount ];
		int vertexCount = 0;
		int indexCount = 0;

		Array3d<bool> topFacesOptimized(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);
		Array3d<bool> bottomFacesOptimized(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);
		Array3d<bool> frontFacesOptimized(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);
		Array3d<bool> backFacesOptimized(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);
		Array3d<bool> leftFacesOptimized(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);
		Array3d<bool> rightFacesOptimized(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);

		topFacesOptimized.Reset(false);
		bottomFacesOptimized.Reset(false);
		frontFacesOptimized.Reset(false);
		backFacesOptimized.Reset(false);
		leftFacesOptimized.Reset(false);
		rightFacesOptimized.Reset(false);

		for ( int x = 0; x < CHUNK_SIZE_X ; ++x)
		{
			for ( int z = 0; z < CHUNK_SIZE_Z ; ++z)
			{
				for ( int y = 0; y < CHUNK_SIZE_Y ; ++y)
				{
					if( vertexCount > USHRT_MAX )
						break ;

					BlockType bt = GetBloc (x, y, z);

					// Regarde si chaque face à besoin d'être optimisé car
					// l'algorithme d'optimisation doit commencer sur une face
					// visible
					if (bt == BTYPE_AIR)
					{
						topFacesOptimized.Set(x,y,z,true);
						bottomFacesOptimized.Set(x,y,z,true);
						frontFacesOptimized.Set(x,y,z,true);
						backFacesOptimized.Set(x,y,z,true);
						leftFacesOptimized.Set(x,y,z,true);
						rightFacesOptimized.Set(x,y,z,true);
					}
					else
					{
						// Bottom
						if (y == 0 || GetBloc(x, y - 1, z) != BTYPE_AIR)
						{
							bottomFacesOptimized.Set(x,y,z,true);
						}

						// Top
						if (y == CHUNK_SIZE_Y - 1 || GetBloc(x, y + 1, z) != BTYPE_AIR)
						{
							topFacesOptimized.Set(x,y,z,true);
						}

						// Front
						if (z == 0)
						{
							if(!frontChunk || frontChunk->GetBloc(x, y, CHUNK_SIZE_Z - 1) != BTYPE_AIR)
								frontFacesOptimized.Set(x,y,z,true);
						}
						else if (GetBloc(x, y, z - 1) != BTYPE_AIR)
						{
							frontFacesOptimized.Set(x,y,z,true);
						}

						// Back
						if (z == CHUNK_SIZE_Z - 1)
						{
							if(!backChunk || backChunk->GetBloc(x, y, 0) != BTYPE_AIR)
								backFacesOptimized.Set(x,y,z,true);
						}
						else if (GetBloc(x, y, z + 1) != BTYPE_AIR)
						{
							backFacesOptimized.Set(x,y,z,true);
						}

						// Left
						if (x == 0)
						{
							if(!leftChunk || leftChunk->GetBloc(CHUNK_SIZE_X - 1, y, z) != BTYPE_AIR)
								leftFacesOptimized.Set(x,y,z,true);
						}
						else if (GetBloc(x - 1, y, z) != BTYPE_AIR)
						{
							leftFacesOptimized.Set(x,y,z,true);
						}

						// Right
						if (x == CHUNK_SIZE_X - 1)
						{
							if(!rightChunk || rightChunk->GetBloc(0, y, z) != BTYPE_AIR)
								rightFacesOptimized.Set(x,y,z,true);
						}
						else if (GetBloc(x + 1, y, z) != BTYPE_AIR)
						{
							rightFacesOptimized.Set(x,y,z,true);
						}
					}

					// Optimise les faces qui en ont besoin
					if (!topFacesOptimized.Get(x,y,z))	
					{
						CreateOptimizedTopBottomFace(FACE_TOP, vd, vertexCount, td, x, y, z, bt, topFacesOptimized);
					}
					if (!bottomFacesOptimized.Get(x,y,z))	
					{
						CreateOptimizedTopBottomFace(FACE_BOTTOM, vd, vertexCount, td, x, y, z, bt, topFacesOptimized);
					}
					if (!frontFacesOptimized.Get(x,y,z))
					{
						CreateOptimizedFrontBackFace(FACE_FRONT, vd, vertexCount, td, x, y, z, bt, topFacesOptimized);
					}
					if (!backFacesOptimized.Get(x,y,z))
					{
						CreateOptimizedFrontBackFace(FACE_BACK, vd, vertexCount, td, x, y, z, bt, topFacesOptimized);
					}
					if (!leftFacesOptimized.Get(x,y,z))
					{
						CreateOptimizedLeftRightFace(FACE_LEFT, vd, vertexCount, td, x, y, z, bt, topFacesOptimized);
					}
					if (!rightFacesOptimized.Get(x,y,z))
					{
						CreateOptimizedLeftRightFace(FACE_RIGHT, vd, vertexCount, td, x, y, z, bt, topFacesOptimized);
					}
				}
			}
		}
		if( vertexCount > USHRT_MAX )
		{
			vertexCount = USHRT_MAX ;
			std :: cout << "[ Chunk :: Update ] Chunk data truncaned , too much vertices to have a 16 bit index " << std :: endl ;
		}
		m_chunkMesh . SetMeshData (vd , vertexCount, td);
		delete [] vd;
		delete [] id;
		delete [] td;
	}
	m_isDirty = false ;
}

void Chunk::Render() const
{
	m_chunkMesh.Render();
}

void Chunk::CreateOptimizedTopBottomFace(MeshFace face, ChunkMesh::VertexData* vd, int& vertexCount, ChunkMesh::TextureData* td, int x, int y, int z, BlockType bt, Array3d<bool>& facesOptimized)
{
	// Itérateurs : i = x, j = z
	// Conditions d'arrêt : 
	//  - On est arrivé à la fin du chunk (CHUNK_SIZE - 1)
	//  - Le prochain bloc n'est pas du même type que @bt
	//  - Le bloc au dessus du prochain bloc n'est pas de l'air
	//  - Le prochain bloc est déja optimisé

	int endX = -1;
	int endZ = -1;

	// Parcourt le chunk en x
	for(int i = x; i < CHUNK_SIZE_X; ++i)
	{
		// Parcourt le chunk en z
		for(int j = z; j < CHUNK_SIZE_Z; ++j)
		{
			// Si une des conditions d'arrêt est vraie
			if ((j == CHUNK_SIZE_Z - 1) || (GetBloc(i, y, j + 1) != bt) || (GetBloc(i, y + 1, j + 1) != BTYPE_AIR) || facesOptimized.Get(i, y, j + 1))
			{
				// Trouve la plus petite position en Y qui répond à la condition d'arrêt
				if (endZ == -1 || j < endZ)
					endZ = j;
				break;
			}
		}
		// Si une des conditions d'arrêt est vraie
		if ((i == CHUNK_SIZE_X - 1) || (GetBloc(i + 1, y, z) != bt) || (GetBloc(i + 1, y + 1, z) != BTYPE_AIR) || facesOptimized.Get(i + 1, y, z))
		{
			endX = i;
			break;
		}
	}

	// Update le tableau avec les nouveaux blocs optimisés
	for (int i = x; i <= endX; ++i)
	{
		for (int j = z; j <= endZ; ++j)
		{
			facesOptimized.Set(i, y, j, true);
		}
	}

	// Dimensions du rectangle
	int w = endX - x + 1;
	int h = endZ - z + 1;

	// Place la face relativement au monde
	Vector2f pos = GetRealPosition();
	x += pos.x;
	y -= 2.f;
	z += pos.y;

	// Détermine les coords de la texture du type de bloc
	BlockInfo::TextureCoords coords = Info::Get().GetBlocInfo(bt)->GetTextureCoords();

	int index = coords.w;

	// Crée les vertices de la face
	if(face == FACE_TOP)
	{
		td[vertexCount] = ChunkMesh::TextureData(0, 0, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + 0.5f, z + h-1 + 0.5f, 1.f, 1.f, 1.f);		//0
		td[vertexCount] = ChunkMesh::TextureData(w, 0, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y + 0.5f, z + h-1 + 0.5f, 1.f, 1.f, 1.f);	//1
		td[vertexCount] = ChunkMesh::TextureData(w, h, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f, 1.f);		//2
		td[vertexCount] = ChunkMesh::TextureData(0, h, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f, 1.f);				//3
	}
	else if(face == FACE_BOTTOM)
	{
		td[vertexCount] = ChunkMesh::TextureData(0, 0, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z + 0.5f + h-1, 0.6f, 0.6f, 0.6f);		//0
		td[vertexCount] = ChunkMesh::TextureData(w, 0, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f);		//1
		td[vertexCount] = ChunkMesh::TextureData(w, h, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y - 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f);		//2
		td[vertexCount] = ChunkMesh::TextureData(0, h, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y - 0.5f, z + 0.5f + h-1, 0.6f, 0.6f, 0.6f);		//3
	}
	else
	{
		std::cout << "invalid face" << std::endl;
	}
}

void Chunk::CreateOptimizedFrontBackFace(MeshFace face, ChunkMesh::VertexData* vd, int& vertexCount, ChunkMesh::TextureData* td, int x, int y, int z, BlockType bt, Array3d<bool>& facesOptimized)
{
	// Itérateurs : i = x, j = y
	// Conditions d'arrêt : 
	//  - On est arrivé à la fin du chunk (CHUNK_SIZE - 1)
	//  - Le prochain bloc n'est pas du même type que @bt
	//  - Le bloc au dessus du prochain bloc n'est pas de l'air
	//  - Le prochain bloc est déja optimisé

	int endX = -1;
	int endY = -1;

	// Parcourt le chunk en x
	for(int i = x; i < CHUNK_SIZE_X; ++i)
	{
		// Parcourt le chunk en y
		for(int j = y; j < CHUNK_SIZE_Y; ++j)
		{
			// Si une des conditions d'arrêt est vraie
			if ((j == CHUNK_SIZE_Y - 1) || (GetBloc(i, j + 1, z) != bt) || (GetBloc(i, j + 1, z) != BTYPE_AIR) || facesOptimized.Get(i, j + 1, z))
			{
				// Trouve la plus petite position en Y qui répond à la condition d'arrêt
				if (endY == -1 || j < endY)
					endY = j;
				break;
			}
		}
		// Si une des conditions d'arrêt est vraie
		if ((i == CHUNK_SIZE_X - 1) || (GetBloc(i + 1, y, z) != bt) || (GetBloc(i + 1, y + 1, z) != BTYPE_AIR) || facesOptimized.Get(i + 1, y, z))
		{
			endX = i;
			break;
		}
	}

	// Update le tableau avec les nouveaux blocs optimisés
	for (int i = x; i <= endX; ++i)
	{
		for (int j = y; j <= endY; ++j)
		{
			facesOptimized.Set(i, j, z, true);
		}
	}

	// Dimensions du rectangle
	int w = endX - x + 1;
	int h = endY - y + 1;

	// Place la face relativement au monde
	Vector2f pos = GetRealPosition();
	x += pos.x;
	y -= 2.f;
	z += pos.y;

	// Détermine les coords de la texture du type de bloc
	BlockInfo::TextureCoords coords = Info::Get().GetBlocInfo(bt)->GetTextureCoords();
	int index = coords.w;

	// Crée les vertices de la face
	if(face == FACE_FRONT)
	{
		td[vertexCount] = ChunkMesh::TextureData(0, 0, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + h-1 + 0.5f, z + 0.5f, 0.9f, 0.9f, 0.9f);			//0
		td[vertexCount] = ChunkMesh::TextureData(w, 0, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z + 0.5f, 0.9f, 0.9f, 0.9f);				//1
		td[vertexCount] = ChunkMesh::TextureData(w, h, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y - 0.5f, z + 0.5f, 0.9f, 0.9f, 0.9f);			//2
		td[vertexCount] = ChunkMesh::TextureData(0, h, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y + h-1 + 0.5f, z + 0.5f, 0.9f, 0.9f, 0.9f);	//3

	}
	else if(face == FACE_BACK)
	{
		td[vertexCount] = ChunkMesh::TextureData(0, 0, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + h-1 + 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f);				//0
		td[vertexCount] = ChunkMesh::TextureData(w, 0, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y + h-1 + 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f);		//1
		td[vertexCount] = ChunkMesh::TextureData(w, h, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y - 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f);				//2
		td[vertexCount] = ChunkMesh::TextureData(0, h, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f);					//3
	}
	else
	{
		std::cout << "invalid face" << std::endl;
	}
}

void Chunk::CreateOptimizedLeftRightFace(MeshFace face, ChunkMesh::VertexData* vd, int& vertexCount, ChunkMesh::TextureData* td, int x, int y, int z, BlockType bt, Array3d<bool>& facesOptimized)
{
	// Itérateurs : i = z, j = y
	// Conditions d'arrêt : 
	//  - On est arrivé à la fin du chunk (CHUNK_SIZE - 1)
	//  - Le prochain bloc n'est pas du même type que @bt
	//  - Le bloc au dessus du prochain bloc n'est pas de l'air
	//  - Le prochain bloc est déja optimisé

	int endZ = -1;
	int endY = -1;

	// Parcourt le chunk en z
	for(int i = z; i < CHUNK_SIZE_Z; ++i)
	{
		// Parcourt le chunk en y
		for(int j = y; j < CHUNK_SIZE_Y; ++j)
		{
			// Si une des conditions d'arrêt est vraie
			if ((j == CHUNK_SIZE_Y - 1) || (GetBloc(i, j + 1, z) != bt) || (GetBloc(i, j + 1, z) != BTYPE_AIR) || facesOptimized.Get(i, j + 1, z))
			{
				// Trouve la plus petite position en Y qui répond à la condition d'arrêt
				if (endY == -1 || j < endY)
					endY = j;
				break;
			}
		}
		// Si une des conditions d'arrêt est vraie
		if ((i == CHUNK_SIZE_Z - 1) || (GetBloc(x, y, i + 1) != bt) || (GetBloc(x, y + 1, i + 1) != BTYPE_AIR) || facesOptimized.Get(x, y, i + 1))
		{
			endZ = i;
			break;
		}
	}

	// Update le tableau avec les nouveaux blocs optimisés
	for (int i = z; i <= endZ; ++i)
	{
		for (int j = y; j <= endY; ++j)
		{
			facesOptimized.Set(x, j, i, true);
		}
	}

	// Dimensions du rectangle
	int w = endZ - z + 1;
	int h = endY - y + 1;

	// Détermine les coords de la texture du type de bloc
	BlockInfo::TextureCoords coords = Info::Get().GetBlocInfo(bt)->GetTextureCoords();
	int index = coords.w;

	// Place la face relativement au monde
	Vector2f pos = GetRealPosition();
	x += pos.x;
	y -= 2.f;
	z += pos.y;

	// Détermine les coords de la texture du type de bloc
	float x0 = coords.u;
	float y0 = coords.v;
	float x1 = coords.u + coords.w;
	float y1 = coords.v + coords.h;

	// Crée les vertices de la face
	if(face == FACE_LEFT)
	{
		td[vertexCount] = ChunkMesh::TextureData(0, 0, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z + w-1 + 0.5f, 0.8f, 0.8f, 0.8f);			//0
		td[vertexCount] = ChunkMesh::TextureData(w, 0, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + h-1 + 0.5f, z + w-1 + 0.5f, 0.8f, 0.8f, 0.8f);	//1
		td[vertexCount] = ChunkMesh::TextureData(w, h, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + h-1 + 0.5f, z - 0.5f, 0.8f, 0.8f, 0.8f);			//2
		td[vertexCount] = ChunkMesh::TextureData(0, h, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z - 0.5f, 0.8f, 0.8f, 0.8f);				//3
	}
	else if(face == FACE_RIGHT)
	{
		td[vertexCount] = ChunkMesh::TextureData(0, 0, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y - 0.5f, z + w-1 + 0.5f, 0.6f, 0.6f, 0.6f);		//0
		td[vertexCount] = ChunkMesh::TextureData(h, 0, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y - 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f);		//1
		td[vertexCount] = ChunkMesh::TextureData(h, w, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y + h-1 + 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f);		//2
		td[vertexCount] = ChunkMesh::TextureData(0, w, index);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y + h-1 + 0.5f, z + w-1 + 0.5f, 0.6f, 0.6f, 0.6f);		//3
	}
	else
	{
		std::cout << "invalid face" << std::endl;
	}
}

Vector2i Chunk::GetPosition() const
{
	return m_pos;
}

Vector2f Chunk::GetRealPosition() const
{
	return Vector2f(m_pos.x * CHUNK_SIZE_X - VIEW_DISTANCE, m_pos.y * CHUNK_SIZE_Z - VIEW_DISTANCE);
}

void Chunk::SetPosition( Vector2i pos )
{
	m_pos = pos;
}