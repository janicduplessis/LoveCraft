#include "chunkloader.h"
#include "info.h"

#include <iostream>

ChunkLoader::ChunkLoader(sf::Mutex* mutex) : m_thread(0), m_loading(false), m_maxViewPosX(VIEW_DISTANCE), m_minViewPosX(-VIEW_DISTANCE),
	m_maxViewPosY(VIEW_DISTANCE), m_minViewPosY(-VIEW_DISTANCE), m_mutex(mutex)
{

}

ChunkLoader::~ChunkLoader()
{

}

void ChunkLoader::CheckPlayerPosition( Player* player )
{
	if (m_loading)
		return;

	Array2d<Chunk*>* chunks = Info::Get().GetChunkArray();
	Vector2i& size = chunks->Size();

	std::cout << player->Position().z << std::endl;
	std::cout << VIEW_DISTANCE + Info::Get().GetOffsetMap().y * CHUNK_SIZE_Z << std::endl;

	if (player->Position().z > VIEW_DISTANCE + Info::Get().GetOffsetMap().y * CHUNK_SIZE_Z + CHUNK_SIZE_Z && !m_loading) {
		m_loading = true;
		//delete m_thread;
		//m_thread = new sf::Thread(LoadFrontChunks(m_mutex, m_loading));
		//m_thread->launch();
		LoadFrontChunks l(m_mutex, m_loading);
		l();
	}
	if (player->Position().z < VIEW_DISTANCE + Info::Get().GetOffsetMap().y * CHUNK_SIZE_Z - CHUNK_SIZE_Z && !m_loading) {
		m_loading = true;
		//delete m_thread;
		//m_thread = new sf::Thread(LoadFrontChunks(m_loading));
		//m_thread->launch();
		//LoadBackChunks l(m_loading);
		//l();
	}
	/*else if (abs(playerPos.x - (Info::Get().GetChunkArray()->Get(0, 0)->GetRealPosition()).x) < VIEW_DISTANCE) {
	m_loading = true;
	delete m_thread;
	m_thread = new sf::Thread(LoadBackChunks(m_loading));
	m_thread->launch();
	}*/
}


void LoadFrontChunks::operator()()
{
	m_mutex->lock();
	Array2d<Chunk*>* chunks = Info::Get().GetChunkArray();
	Vector2i& size = chunks->Size();

	// Down Shift
	for (unsigned int x = 0; x < size.x; ++x)
	{
		delete (chunks->Get(x, 0));
	}
	for (unsigned int y = 1; y < (size.y); ++y)
	{
		for (unsigned int x = 0; x < size.x; ++x)
		{
			Chunk* c = chunks->Get(x, y);
			chunks->Set(x, y - 1, c);
		}
	}
	for (unsigned int x = 0; x < size.x; ++x)
	{
		chunks->Set(x, size.y - 1, 0);
	}

	Info::Get().SetOffsetMap(Info::Get().GetOffsetMap() + Vector2i(0,1));

	m_mutex->unlock();

	// Creer les nouveau chunks
	Chunk** newChunks = new Chunk*[size.x];
	for (unsigned int x = 0; x < size.x; ++x)
	{
		Vector2f& pos = chunks->Get(x, size.y - 2)->GetWorldPosition();
		newChunks[x] = new Chunk(Vector2i(x, size.y - 1), Vector2f(pos.x, pos.y + 1));
	}

	m_mutex->lock();

	for (unsigned int x = 0; x < size.x; ++x)
	{
		chunks->Set(x, size.y - 1, newChunks[x]);
	}
	for (unsigned int x = 0; x < size.x; ++x)
	{
		for (unsigned int y = 0; y < size.y; ++y)
		{
			chunks->Get(x, y)->SetSurroundings(Vector2i(x, y));
		}
	}
	for (unsigned int x = 0; x < size.x; ++x)
	{
		chunks->Get(x, size.y - 2)->GenerateTrees();
		//chunks->Get(x, size.y - 1)->Update();
		chunks->Get(x, size.y - 1)->SetIsReady(true);
	}

	delete [] newChunks;

	m_loading = false;
	m_mutex->unlock();
}

void LoadBackChunks::operator()()
{
	sf::Mutex mutex;
	mutex.lock();
	Array2d<Chunk*>* chunks = Info::Get().GetChunkArray();
	Vector2i& size = chunks->Size();

	// Up Shift
	for (unsigned int x = 0; x < size.x; ++x)
	{
		delete (chunks->Get(x, size.y - 1));
	}

	for (int y = size.y - 2; y >= 0; --y)
	{
		for (unsigned int x = 0; x < size.x; ++x)
		{
			Chunk* c = chunks->Get(x, y);
			chunks->Set(x, y + 1, c);
		}
	}

	for (unsigned int x = 0; x < size.x; ++x)
	{
		chunks->Set(x, 0, 0);
	}

	Info::Get().SetOffsetMap(Info::Get().GetOffsetMap() + Vector2i(0,-1));

	// Creer les nouveau chunks
	Chunk** newChunks = new Chunk*[size.x];
	for (unsigned int x = 0; x < size.x; ++x)
	{
		Vector2f& pos = chunks->Get(x, 1)->GetWorldPosition();
		newChunks[x] = new Chunk(Vector2i(x, 0), Vector2f(pos.x, pos.y - 1));
	}

	for (unsigned int x = 0; x < size.x; ++x)
	{
		chunks->Set(x, 0, newChunks[x]);
	}
	for (unsigned int x = 0; x < size.x; ++x)
	{
		for (unsigned int y = 0; y < size.y; ++y)
		{
			chunks->Get(x, y)->SetSurroundings(Vector2i(x, y));
		}
	}


	for (unsigned int x = 0; x < size.x; ++x)
	{
		chunks->Get(x, 0)->Update();
		chunks->Get(x, 0)->SetIsReady(true);
	}

	delete [] newChunks;

	m_loading = false;

	mutex.unlock();
}

void LoadLeftChunks::operator()()
{

}

void LoadRightChunks::operator()()
{

}
