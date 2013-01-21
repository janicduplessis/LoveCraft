#ifndef TREE_GENERATOR_H__
#define TREE_GENERATOR_H__

#include "define.h"
#include "chunk.h"

#include <util/vector3.h>
#include <util/perlin.h>

#define MAX_TRUNK_LENGHT 12
#define MIN_TRUNK_LENGHT 7
#define MAX_FOILIAGE_SIZE 6
#define MIN_FOILIAGE_SIZE 3

class TreeGenerator
{
public:
	TreeGenerator(long long seed);
	~TreeGenerator();

	void GenerateTree(const Vector3f& pos, Chunk* c, unsigned int seed);

private:
	Perlin m_perlin;
};


#endif // !TREE_GENERATOR_H__
