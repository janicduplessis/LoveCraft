#include "npc.h"

Npc::Npc(const Vector3f& pos /*= Vector3f(0,0,0)*/ )
{

}

Npc::~Npc()
{

}

void Npc::Init()
{

}

void Npc::Update()
{
	m_model.SetPosition(m_pos);
}

void Npc::Render() const
{
	m_model.Render();
}
