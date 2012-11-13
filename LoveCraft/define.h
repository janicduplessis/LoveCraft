/*
 *	define.h
 *
 *	Created on: 2012-10-31
 *	Author: Janic Duplessis
 */

#include <iostream>

#ifndef BLOC_TYPE
#define BLOC_TYPE
enum BlocType
{
	BTYPE_AIR,
	BTYPE_DIRT,
	BTYPE_GRASS
};
#endif // !BLOC_TYPE

#ifndef TRACE
#define TRACE(x) std::cout << __FILE__ << ":" << __LINE__ << ": " << x << std::endl
#endif // !TRACE

#ifndef CHUNK_SIZE_X
#define CHUNK_SIZE_X 16
#endif // !CHUNK_SIZE_X

#ifndef CHUNK_SIZE_Y
#define CHUNK_SIZE_Y 128
#endif // !CHUNK_SIZE_Y

#ifndef CHUNK_SIZE_Z
#define CHUNK_SIZE_Z 16
#endif // !CHUNK_SIZE_Z

