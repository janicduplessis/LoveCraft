#include "treegenerator.h"

TreeGenerator::TreeGenerator( long long seed ) : m_perlin(8, 8, 1, seed)
{

}

TreeGenerator::~TreeGenerator()
{

}

void TreeGenerator::GenerateTree(const  Vector3f& pos, Chunk* c, unsigned int seed)
{
	srand(seed);
	int trunkHeight = MIN_TRUNK_LENGHT + rand() % (MAX_TRUNK_LENGHT - MIN_TRUNK_LENGHT);
	int foiliageStart = trunkHeight - 3;
	int foiliageSize = MIN_FOILIAGE_SIZE + rand() % (MAX_FOILIAGE_SIZE - MIN_FOILIAGE_SIZE);
	int foiliageEnd = foiliageStart + foiliageSize * 2;

	// Create trunk
	for (Vector3f i = pos; i.y < pos.y + trunkHeight; i.y++)
		c->SetBloc(i.x, i.y, i.z, BTYPE_TREETRUNK);

	Perlin perlin(6,5,1,seed);

	// Leafes
	for (int y = pos.y + foiliageStart; y < pos.y + foiliageEnd; ++y)
	{
		for (int x = pos.x - foiliageSize; x < pos.x + foiliageSize; ++x)
		{
			for (int z = pos.z - foiliageSize; z < pos.z + foiliageSize; ++z)
			{
				float val = perlin.Get((x - pos.x) / 2000.f, (y - pos.y) / 2000.f, (z - pos.z) / 2000.f);
				//std::cout << val << std::endl;
				if (val < -0.03f && val > -0.1f && c->GetBloc(x,y,z) == BTYPE_AIR)
					c->SetBloc(x,y,z,BTYPE_TREELEAF);
			}
		}
	}
}
