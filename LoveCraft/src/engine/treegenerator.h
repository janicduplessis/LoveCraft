#ifndef TREE_GENERATOR_H__
#define TREE_GENERATOR_H__

#include "define.h"
#include "chunk.h"

#include <util/vector3.h>
#include <util/perlin.h>

#define MAX_TRUNK_LENGHT 13
#define MIN_TRUNK_LENGHT 8
#define MAX_FOILIAGE_SIZE 6
#define MIN_FOILIAGE_SIZE 3

class TreeGenerator
{
public:
	TreeGenerator();
	~TreeGenerator();

	void GenerateTree(const Vector3f& pos, Chunk* c, unsigned int seed);

private:
};


#endif // !TREE_GENERATOR_H__
