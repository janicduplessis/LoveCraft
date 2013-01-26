#include "chunk.h"
#include <util/perlin.h>
#include "treegenerator.h"

Chunk::Chunk(const Vector2i& arrayPos, const Vector2f& worldPos) : m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), m_chunkMesh(Info::Get().GetCubeShader()),
	m_left(0), m_right(0), m_back(0), m_front(0), m_chunkArrayPos(arrayPos), m_worldPos(worldPos), m_isReady(false)
{
	m_blocks.Reset(BTYPE_AIR);
	// The first parameter is the number of octaves , this is how noisy or smooth the function is. This is valid between 1 and 16. A value of
	// 4 to 8 octaves produces fairly conventional noise results . The second parameter is the noise frequency . Values betwen 1 and 8 are
	// reasonable here . You can try sampling the data and plotting it to the screen to see what numbers you like . The last parameter is
	// the amplitude . Setting this to a value of 1 will return randomized samples between -1 and +1. The last parameter is the random
	// number seed .
	Perlin perlin(16 ,6 ,1 ,95);
	Perlin perlinElevation(16, 5, 1, 76);

	for ( int x = 0; x < CHUNK_SIZE_X ; ++x)
	{
		for ( int z = 0; z < CHUNK_SIZE_Z ; ++z)
		{
			// La methode Get accepte deux param^etre ( coordonnee en X et Z) et retourne une valeur qui respecte
			// les valeurs utilisees lors de la creation de l' objet Perlin
			// La valeur retournee est entre 0 et 1
			float valDetail = perlin . Get (( float )( m_worldPos.x * CHUNK_SIZE_X + x) / 2000.f, ( float )( m_worldPos.y * CHUNK_SIZE_Z + z) / 2000.f);
			int yMax = 64 + (int)((valDetail) * 55.f);
			for(int y = 0; y <= yMax; y++)
			{
				if (y == yMax) {
					m_blocks.Set(x,y,z, BTYPE_GRASS);
				}
				else
					m_blocks.Set(x,y,z, BTYPE_DIRT);

			}
			// Utiliser val pour determiner la hauteur du terrain a la position x,z
			// Vous devez vous assurer que la hauteur ne depasse pas CHUNK_SIZE_Y
			// Remplir les blocs du bas du terrain (le 'bed rock ') jusqu 'a la hauteur calculee .
			// N' hesitez pas a jouer avec la valeur retournee pour obtenir un resultat qui vous semble satisfaisant
		}
	}
}

Chunk::~Chunk()
{
}

void Chunk::GenerateTrees()
{
	if (!m_left || !m_right || !m_front || !m_back)
		return;

	TreeGenerator treeGen;

	for ( int x = 0; x < CHUNK_SIZE_X ; ++x)
	{
		for ( int z = 0; z < CHUNK_SIZE_Z ; ++z)
		{
			for(int y = 0; y < CHUNK_SIZE_Y; y++)
			{
				if (m_blocks.Get(x,y,z) == BTYPE_GRASS && NoCloseTree(Vector3f(x,y,z)) && rand() % 30 == 0) {
					treeGen.GenerateTree(Vector3f(x, y, z), this, m_worldPos.x * CHUNK_SIZE_X + x * m_worldPos.y * CHUNK_SIZE_Z + z);
				}
			}
		}
	}
}

bool Chunk::NoCloseTree(Vector3f pos)
{
	for ( int x = pos.x - 5; x < pos.x + 5; ++x)
	{
		for ( int z = pos.z - 5; z < pos.z + 5 ; ++z)
		{
			for(int y = pos.y - 5; y < pos.y + 5; y++)
			{
				if(GetBloc(x,y,z) == BTYPE_TREETRUNK)
					return false;
			}
		}
	}
	return true;
}

void Chunk::RemoveBloc(uint32 x, uint32 y, uint32 z)
{
	m_blocks.Set(x, y, z, BTYPE_AIR);
}

void Chunk::SetBloc(int x, int y, int z, BlockType type)
{
	Chunk* editedChunk = this;

	// Front
	if (z < 0)
	{
		if(m_back)
			m_back->SetBloc(x, y, CHUNK_SIZE_Z + z, type);
	}
	// Back
	else if (z >= CHUNK_SIZE_Z)
	{
		if(m_front)
			m_front->SetBloc(x, y, z - CHUNK_SIZE_Z, type);
	}
	// Left
	else if (x < 0)
	{
		if(m_left)
			m_left->SetBloc(CHUNK_SIZE_X + x, y, z, type);
	}
	// Right
	else if (x >= CHUNK_SIZE_X)
	{
		if(m_right)
			m_right->SetBloc(x - CHUNK_SIZE_X, y, z, type);
	}
	else
	{
		m_blocks.Set(x, y, z, type);
	}
	editedChunk->m_isDirty = true;

	if (z == 0 && m_front)
		m_front->m_isDirty = true;
	else if (z == CHUNK_SIZE_Z - 1 && m_back)
		m_back->m_isDirty = true;
	else if (x == 0 && m_left)
		m_left->m_isDirty = true;
	else if (x == CHUNK_SIZE_X - 1 && m_right)
		m_right->m_isDirty = true;
}

BlockType Chunk::GetBloc(int x, int y, int z)
{
	BlockType result;

	// Bottom et top
	if (y == -1 || y >= CHUNK_SIZE_Y)
	{
		result = BTYPE_AIR;
	}
	// Front
	else if (z < 0)
	{
		if(m_back)
			result = m_back->GetBloc(x, y, CHUNK_SIZE_Z + z);
		else
			result = BTYPE_AIR;
	}
	// Back
	else if (z >= CHUNK_SIZE_Z)
	{
		if(m_front)
			result = m_front->GetBloc(x, y, z - CHUNK_SIZE_Z);
		else
			result = BTYPE_AIR;
	}
	// Left
	else if (x < 0)
	{
		if(m_left)
			result = m_left->GetBloc(CHUNK_SIZE_X + x, y, z);
		else
			result = BTYPE_AIR;
	}
	// Right
	else if (x >= CHUNK_SIZE_X)
	{
		if(m_right)
			result = m_right->GetBloc(x - CHUNK_SIZE_X, y, z);
		else
			result = BTYPE_AIR;
	}
	else
		result = m_blocks.Get(x, y, z);

	return result;
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
		ChunkMesh::TextureData* td = new ChunkMesh::TextureData[ maxVertexCount ];
		int vertexCount = 0;

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
					if (bt == BTYPE_AIR || y == 0)
					{
						bottomFacesOptimized.Set(x,y,z,true);
						frontFacesOptimized.Set(x,y,z,true);
						backFacesOptimized.Set(x,y,z,true);
						leftFacesOptimized.Set(x,y,z,true);
						rightFacesOptimized.Set(x,y,z,true);
					}
					if (bt == BTYPE_AIR)
						topFacesOptimized.Set(x,y,z,true);

					if (GetBloc(x, y + 1, z) != BTYPE_AIR)
						topFacesOptimized.Set(x,y,z, true);
					if (GetBloc(x, y - 1, z) != BTYPE_AIR)
						bottomFacesOptimized.Set(x,y,z, true);
					if (GetBloc(x, y , z + 1) != BTYPE_AIR)
						backFacesOptimized.Set(x,y,z, true);
					if (GetBloc(x, y, z - 1) != BTYPE_AIR)
						frontFacesOptimized.Set(x,y,z, true);
					if (GetBloc(x - 1, y, z) != BTYPE_AIR)
						leftFacesOptimized.Set(x,y,z, true);
					if (GetBloc(x + 1, y, z) != BTYPE_AIR)
						rightFacesOptimized.Set(x,y,z, true);

					// Optimise les faces qui en ont besoin
					if (!topFacesOptimized.Get(x,y,z))	
					{
						CreateOptimizedTopBottomFace(FACE_TOP, vd, vertexCount, td, x, y, z, bt, topFacesOptimized);
					}
					if (!bottomFacesOptimized.Get(x,y,z))	
					{
						CreateOptimizedTopBottomFace(FACE_BOTTOM, vd, vertexCount, td, x, y, z, bt, bottomFacesOptimized);
					}
					if (!frontFacesOptimized.Get(x,y,z))
					{
						CreateOptimizedFrontBackFace(FACE_FRONT, vd, vertexCount, td, x, y, z, bt, frontFacesOptimized);
					}
					if (!backFacesOptimized.Get(x,y,z))
					{
						CreateOptimizedFrontBackFace(FACE_BACK, vd, vertexCount, td, x, y, z, bt, backFacesOptimized);
					}
					if (!leftFacesOptimized.Get(x,y,z))
					{
						CreateOptimizedLeftRightFace(FACE_LEFT, vd, vertexCount, td, x, y, z, bt, leftFacesOptimized);
					}
					if (!rightFacesOptimized.Get(x,y,z))
					{
						CreateOptimizedLeftRightFace(FACE_RIGHT, vd, vertexCount, td, x, y, z, bt, rightFacesOptimized);
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
			if ((j == CHUNK_SIZE_Z - 1) || (GetBloc(i, y, j + 1) != bt) || (GetBloc(i, y + (face == FACE_BOTTOM) ? -1 : 1, j + 1) != BTYPE_AIR) || facesOptimized.Get(i, y, j + 1))
			{
				// Trouve la plus petite position en Y qui répond à la condition d'arrêt
				if (endZ == -1 || j < endZ)
					endZ = j;
				break;
			}
		}
		// Si une des conditions d'arrêt est vraie
		if ((i == CHUNK_SIZE_X - 1) || (GetBloc(i + 1, y, z) != bt) || (GetBloc(i + 1, y + (face == FACE_BOTTOM) ? -1 : 1, z) != BTYPE_AIR) || facesOptimized.Get(i + 1, y, z))
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
	Vector2f pos = Vector2f(m_worldPos.x * CHUNK_SIZE_X, m_worldPos.y * CHUNK_SIZE_Z);
	x += pos.x;
	y -= 2.f;
	z += pos.y;

	float iTop = Info::Get().GetBlocInfo(bt)->GetTextureIndex(0);
	float iBot = Info::Get().GetBlocInfo(bt)->GetTextureIndex(2);

	// Crée les vertices de la face
	if(face == FACE_TOP)
	{
		td[vertexCount] = ChunkMesh::TextureData(0, 0, iTop);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + 0.5f, z + h-1 + 0.5f, 1.f, 1.f, 1.f);		//0
		td[vertexCount] = ChunkMesh::TextureData(w, 0, iTop);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y + 0.5f, z + h-1 + 0.5f, 1.f, 1.f, 1.f);	//1
		td[vertexCount] = ChunkMesh::TextureData(w, h, iTop);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f, 1.f);		//2
		td[vertexCount] = ChunkMesh::TextureData(0, h, iTop);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f, 1.f);				//3
	}
	else if(face == FACE_BOTTOM)
	{
		td[vertexCount] = ChunkMesh::TextureData(0, 0, iBot);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z + 0.5f + h-1, 0.6f, 0.6f, 0.6f);			//0
		td[vertexCount] = ChunkMesh::TextureData(h, 0, iBot);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f);				//1
		td[vertexCount] = ChunkMesh::TextureData(h, w, iBot);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y - 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f);			//2
		td[vertexCount] = ChunkMesh::TextureData(0, w, iBot);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y - 0.5f, z + 0.5f + h-1, 0.6f, 0.6f, 0.6f);	//3
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
			if ((j == CHUNK_SIZE_Y - 1) || (GetBloc(i, j + 1, z) != bt) || (GetBloc(i, j + 1, z + (face == FACE_BACK) ? -1 : 1) != BTYPE_AIR) || facesOptimized.Get(i, j + 1, z))
			{
				// Trouve la plus petite position en Y qui répond à la condition d'arrêt
				if (endY == -1 || j < endY)
					endY = j;
				break;
			}
		}
		// Si une des conditions d'arrêt est vraie
		if ((i == CHUNK_SIZE_X - 1) || (GetBloc(i + 1, y, z) != bt) || (GetBloc(i + 1, y, z + (face == FACE_BACK) ? -1 : 1) != BTYPE_AIR) || facesOptimized.Get(i + 1, y, z))
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
	Vector2f pos = Vector2f(m_worldPos.x * CHUNK_SIZE_X, m_worldPos.y * CHUNK_SIZE_Z);
	x += pos.x;
	y -= 2.f;
	z += pos.y;
;
	float iFront;
	float iBack;

	iFront = iBack = Info::Get().GetBlocInfo(bt)->GetTextureIndex(1);

	// Crée les vertices de la face
	if(face == FACE_FRONT)
	{
		td[vertexCount] = ChunkMesh::TextureData(0, h, iFront);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + h-1 + 0.5f, z - 0.5f, 0.9f, 0.9f, 0.9f);			//0
		td[vertexCount] = ChunkMesh::TextureData(w, h, iFront);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y + h-1 + 0.5f, z - 0.5f, 0.9f, 0.9f, 0.9f);	//1
		td[vertexCount] = ChunkMesh::TextureData(w, 0, iFront);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y - 0.5f, z - 0.5f, 0.9f, 0.9f, 0.9f);			//2
		td[vertexCount] = ChunkMesh::TextureData(0, 0, iFront);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z - 0.5f, 0.9f, 0.9f, 0.9f);				//3

	}
	else if(face == FACE_BACK)
	{
		td[vertexCount] = ChunkMesh::TextureData(w, h, iBack);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + h-1 + 0.5f, z + 0.5f, 0.6f, 0.6f, 0.6f);				//0
		td[vertexCount] = ChunkMesh::TextureData(w, 0, iBack);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z + 0.5f, 0.6f, 0.6f, 0.6f);					//1
		td[vertexCount] = ChunkMesh::TextureData(0, 0, iBack);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y - 0.5f, z + 0.5f, 0.6f, 0.6f, 0.6f);				//2
		td[vertexCount] = ChunkMesh::TextureData(0, h, iBack);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + w-1 + 0.5f, y + h-1 + 0.5f, z + 0.5f, 0.6f, 0.6f, 0.6f);		//3
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
			if ((j == CHUNK_SIZE_Z - 1) || (GetBloc(x, j + 1, i) != bt)
				|| (GetBloc(x + (face == FACE_LEFT) ? - 1 : 1, j + 1, i) != BTYPE_AIR) 
				|| (facesOptimized.Get(x, j + 1, i)))
			{
				// Trouve la plus petite position en Y qui répond à la condition d'arrêt
				if (endY == -1 || j < endY)
					endY = j;
				break;
			}
		}
		// Si une des conditions d'arrêt est vraie
		if ((i == CHUNK_SIZE_Z - 1) || (GetBloc(x, y, i + 1) != bt) || 
			(GetBloc(x + (face == FACE_LEFT) ? - 1 : 1, y, i + 1) != BTYPE_AIR) || 
			facesOptimized.Get(x, y, i + 1))
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

	// Place la face relativement au monde
	Vector2f pos = Vector2f(m_worldPos.x * CHUNK_SIZE_X, m_worldPos.y * CHUNK_SIZE_Z);
	x += pos.x;
	y -= 2.f;
	z += pos.y;

	float iLeft;
	float iRight;

	iLeft = iRight = Info::Get().GetBlocInfo(bt)->GetTextureIndex(1);

	// Crée les vertices de la face
	if(face == FACE_LEFT)
	{
		td[vertexCount] = ChunkMesh::TextureData(0, 0, iLeft);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z + w-1 + 0.5f, 0.8f, 0.8f, 0.8f);			//0
		td[vertexCount] = ChunkMesh::TextureData(0, h, iLeft);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + h-1 + 0.5f, z + w-1 + 0.5f, 0.8f, 0.8f, 0.8f);	//1
		td[vertexCount] = ChunkMesh::TextureData(w, h, iLeft);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y + h-1 + 0.5f, z - 0.5f, 0.8f, 0.8f, 0.8f);			//2
		td[vertexCount] = ChunkMesh::TextureData(w, 0, iLeft);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x - 0.5f, y - 0.5f, z - 0.5f, 0.8f, 0.8f, 0.8f);				//3
	}
	else if(face == FACE_RIGHT)
	{
		td[vertexCount] = ChunkMesh::TextureData(0, 0, iRight);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y - 0.5f, z + w-1 + 0.5f, 0.6f, 0.6f, 0.6f);			//0
		td[vertexCount] = ChunkMesh::TextureData(w, 0, iRight);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y - 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f);				//1
		td[vertexCount] = ChunkMesh::TextureData(w, h, iRight);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y + h-1 + 0.5f, z - 0.5f, 0.6f, 0.6f, 0.6f);			//2
		td[vertexCount] = ChunkMesh::TextureData(0, h, iRight);
		vd[ vertexCount ++] = ChunkMesh::VertexData (x + 0.5f, y + h-1 + 0.5f, z + w-1 + 0.5f, 0.6f, 0.6f, 0.6f);	//3
	}
	else
	{
		std::cout << "invalid face" << std::endl;
	}
}

Vector2f Chunk::GetWorldPosition() const
{
	return m_worldPos;
}

Vector2i Chunk::GetArrayPosition() const
{
	return m_chunkArrayPos;
}

void Chunk::SetWorldPosition( Vector2f pos )
{
	m_worldPos = pos;
}

void Chunk::SetSurroundings(Vector2i arrayPos)
{
	m_chunkArrayPos = arrayPos;
	Array2d<Chunk*>* chunks = Info::Get().GetChunkArray();

	if (m_chunkArrayPos.x != 0)
		m_left = chunks->Get(m_chunkArrayPos.x - 1, m_chunkArrayPos.y);

	if(m_chunkArrayPos.x != VIEW_DISTANCE / CHUNK_SIZE_X * 2 - 1)
		m_right = chunks->Get(m_chunkArrayPos.x + 1, m_chunkArrayPos.y);

	if (m_chunkArrayPos.y != 0)
		m_back = chunks->Get(m_chunkArrayPos.x, m_chunkArrayPos.y - 1);

	if(m_chunkArrayPos.y != VIEW_DISTANCE / CHUNK_SIZE_Z * 2 - 1)
		m_front = chunks->Get(m_chunkArrayPos.x, m_chunkArrayPos.y + 1);
}

bool Chunk::IsReady() const
{
	return m_isReady;
}

void Chunk::SetIsReady( bool val )
{
	m_isReady = val;
}
