#ifndef CHUNK_LOADER_H__
#define CHUNK_LOADER_H__

#include "define.h"

#include <SFML/System.hpp>
#include "player.h"

class ChunkLoader
{
public:
	ChunkLoader();
	~ChunkLoader();

	virtual void CheckPlayerPosition(Player* player);

private:
	enum ChunkPos {
		CHUNK_BACK,
		CHUNK_FRONT,
		CHUNK_LEFT,
		CHUNK_RIGHT
	};

private:
	bool m_loading;
	sf::Thread* m_thread;
	long long m_maxViewPosY;
	long long m_minViewPosY;
	long long m_maxViewPosX;
	long long m_minViewPosX;
};

/**
 * Functors pour loader les chunks
 */
class LoadChunks
{
public:
	LoadChunks(bool& loading) : m_loading(loading) {}
	virtual void operator()() = 0;
protected:
	bool& m_loading;
};

class LoadFrontChunks : public LoadChunks
{
public:
	LoadFrontChunks(bool& loading) : LoadChunks(loading) {}
	virtual void operator()();
};

class LoadBackChunks : public LoadChunks
{
public:
	LoadBackChunks(bool& loading) : LoadChunks(loading) {}
	virtual void operator()();
};

class LoadLeftChunks : public LoadChunks
{
public:
	LoadLeftChunks(bool& loading) : LoadChunks(loading) {}
	virtual void operator()();
};

class LoadRightChunks : public LoadChunks
{
public:
	LoadRightChunks(bool& loading) : LoadChunks(loading) {}
	virtual void operator()();
};

#endif