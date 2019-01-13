#include "Snek.h"

Snek::Snek(SnekHead* snekHead)
{
	m_po_Head = snekHead;

}

Snek::~Snek()
{
	m_v_BodyParts.clear();
}

void Snek::AddBodyPart(SnekBody* snekBody)
{
	if (m_v_BodyParts.empty())
		snekBody->m_o_Reference = m_po_Head;
	else
		snekBody->m_o_Reference = m_v_BodyParts.back();
	m_v_BodyParts.push_back(snekBody);
}

void Snek::Update()
{
	m_po_Head->Update();
	auto i_BodyParts = m_v_BodyParts.begin();
	for (;i_BodyParts != m_v_BodyParts.end(); ++i_BodyParts)
	{
		(*i_BodyParts)->Update();
	}
}

void Snek::Draw()
{
	auto i_BodyParts = m_v_BodyParts.rbegin();
	for (;i_BodyParts != m_v_BodyParts.rend() ; ++i_BodyParts)
	{
		(*i_BodyParts)->Draw();

	}
	m_po_Head->Draw();
}
