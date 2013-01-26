#ifndef CHUNK_LOADER_H__
#define CHUNK_LOADER_H__

#include "define.h"

#include <SFML/System.hpp>
#include "player.h"

class ChunkLoader
{
public:
	ChunkLoader(sf::Mutex* mutex);
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
	sf::Mutex* m_mutex;
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
	LoadChunks(sf::Mutex* mutex, bool& loading) : m_mutex(mutex), m_loading(loading) {}
	virtual void operator()() = 0;
protected:
	bool& m_loading;
	sf::Mutex* m_mutex;
};

class LoadFrontChunks : public LoadChunks
{
public:
	LoadFrontChunks(sf::Mutex* mutex, bool& loading) : LoadChunks(mutex, loading) {}
	virtual void operator()();
};

class LoadBackChunks : public LoadChunks
{
public:
	LoadBackChunks(sf::Mutex* mutex, bool& loading) : LoadChunks(mutex, loading) {}
	virtual void operator()();
};

class LoadLeftChunks : public LoadChunks
{
public:
	LoadLeftChunks(sf::Mutex* mutex, bool& loading) : LoadChunks(mutex, loading) {}
	virtual void operator()();
};

class LoadRightChunks : public LoadChunks
{
public:
	LoadRightChunks(sf::Mutex* mutex, bool& loading) : LoadChunks(mutex, loading) {}
	virtual void operator()();
};

#endif