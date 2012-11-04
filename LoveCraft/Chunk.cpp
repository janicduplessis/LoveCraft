/*
 *	Chunk.cpp
 *
 *	Created on: 2012-10-31
 *	Author: Janic Duplessis
 */

#include "Chunk.h"

Chunk::Chunk() : BlocArray3d(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z)
{

}

Chunk::Chunk(const Chunk& chunk) : BlocArray3d(chunk){}

void Chunk::RemoveBloc(int x, int y, int z)
{
	BlocArray3d::Set(x, y, z, BTYPE_AIR);
}

void Chunk::SetBloc(int x, int y, int z, BlocType type)
{
	BlocArray3d::Set(x, y, z, type);
}

BlocType Chunk::GetBloc(int x, int y , int z) const
{
	return BlocArray3d::Get(x, y, z);
}

Chunk::~Chunk() {}