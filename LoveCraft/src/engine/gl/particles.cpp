#include "particles.h"
#include <vector>
#include <algorithm>
#include <cassert>
#include <util/tool.h>

Particles::Particles(unsigned int particlesNumber) : m_particlesNumber(particlesNumber), 
	m_pos(0), m_range(0.5f), m_color(0.5f), m_angle(Quaternion(1,0,0,0)), m_averageVelocity(2),
	m_particlesSize(0.1f), m_averageLifespan(1.5), m_texture(0)
{
	m_particles = new Particle[m_particlesNumber];
}

Particles::~Particles()
{
	delete [] m_particles;
}

void Particles::Init()
{
	for(int i = 0; i < m_particlesNumber; ++i) {
		CreateParticle(m_particles + i);
	}
}

void Particles::Update(float elapsedTime)
{
	for (unsigned int i = 0; i < m_particlesNumber; ++i)
	{
		Particle* p = m_particles + i;

		p->pos += p->velocity * elapsedTime;
		p->timeAlive += elapsedTime;
		if (p->timeAlive > p->lifespan) {
			CreateParticle(p);
		}
	}

	std::vector<Particle*> ps;
	for(int i = 0; i < m_particlesNumber; i++) {
		ps.push_back(m_particles + i);
	}
	std::sort(ps.begin(), ps.end(), Particles::CompareParticles);

	VertexData* vd = new VertexData[m_particlesNumber * 4];
	unsigned int vertexCount = 0;

	for(unsigned int i = 0; i < ps.size(); i++) {
		Particle* p = ps[i];

		float size = m_particlesSize / 2;
		Vector3f pos = p->pos;
		float r = p->color.x;
		float g = p->color.y;
		float b = p->color.z;
		float a = p->timeAlive / p->lifespan;

		vd[vertexCount++] = VertexData(pos.x - size, pos.y - size, pos.z, r, b, g, a, 0, 0);
		vd[vertexCount++] = VertexData(pos.x - size, pos.y + size, pos.z, r, b, g, a, 0, 1);
		vd[vertexCount++] = VertexData(pos.x + size, pos.y + size, pos.z, r, b, g, a, 1, 1);
		vd[vertexCount++] = VertexData(pos.x + size, pos.y - size, pos.z, r, b, g, a, 1, 0);
	}

	SetMeshData(vd, vertexCount);

	delete [] vd;
}

void Particles::SetMeshData(VertexData* vd, unsigned int vertexCount)
{
	glewInit();

	assert(vertexCount <= USHRT_MAX);
	if(vertexCount == 0)
		return;

	int indexCount = 0;
	uint16* indexData = new uint16[3 * vertexCount / 2];

	int faceCount = vertexCount / 4.f;
	// Genere les index
	for (int i = 0; i < faceCount; ++i)
	{
		int v = i * 4;
		indexData[indexCount++] = v;
		indexData[indexCount++] = v + 1;
		indexData[indexCount++] = v + 2;
		indexData[indexCount++] = v;
		indexData[indexCount++] = v + 2;
		indexData[indexCount++] = v + 3;
	}
	CHECK_GL_ERROR();

	if(!m_isValid)
	{
		glGenBuffers(1, &m_vertexVboId);
		glGenBuffers(1, &m_indexVboId);
	}

	CHECK_GL_ERROR();

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, vd, GL_STREAM_DRAW);

	m_indicesCount = indexCount;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16)* m_indicesCount, indexData, GL_STREAM_DRAW);

	CHECK_GL_ERROR();

	m_isValid = true;
}

void Particles::Render(bool wireFrame) const
{
	if(IsValid())
	{
		m_texture->Bind();
		//glClientActiveTexture(GL_TEXTURE0);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(VertexData), (char*)0);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, sizeof(VertexData), (char*)12);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData), (char*)28);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
		glDrawElements(wireFrame ? GL_LINES : GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_SHORT, (char*)0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

float Particles::RandomFloat() const
{
	return (float)rand() / 2.f;
}

bool Particles::CompareParticles( Particle* particle1, Particle* particle2 )
{
	return particle1->pos.z < particle2->pos.z;
}

Vector3f Particles::AvgVelocity() const
{
	return m_angle * Vector3f(-1,0,0) * m_averageVelocity;
}

void Particles::CreateParticle( Particle* p ) const
{
	p->pos = m_pos;
	p->velocity = AvgVelocity() + Vector3f(m_range * RandomFloat() - m_range / 2,
										  m_range * RandomFloat() - m_range / 2,
										  m_range * RandomFloat() - m_range / 2);
	p->color = m_color;
	p->timeAlive = 0;
	p->lifespan = m_averageLifespan + RandomFloat() - 0.5f;
}

void Particles::SetPosition( const Vector3f& pos )
{
	m_pos = pos;
}

void Particles::SetRotation( const Quaternion& q )
{
	m_angle = q;
}

void Particles::SetParticlesNumber( float nbr )
{
	m_particlesNumber = nbr;
}

void Particles::SetRange( float range )
{
	m_range = range;
}

void Particles::SetParticlesSize( float size )
{
	m_particlesSize = size;
}

void Particles::SetTexture( Texture* texture )
{
	m_texture = texture;
}

void Particles::SetColor( const Vector3f& color )
{
	m_color = color;
}

void Particles::SetAverageLifespan( float lifespan )
{
	m_averageLifespan = lifespan;
}

void Particles::SetAverageVelocity( float velocity )
{
	m_averageVelocity = velocity;
}

