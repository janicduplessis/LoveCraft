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

	// Create trunk
	for (Vector3f i = pos; i.y < pos.y + trunkHeight; i.y++)
		c->SetBloc(i.x, i.y, i.z, BTYPE_TREETRUNK);
}
