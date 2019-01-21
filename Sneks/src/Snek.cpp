#include "Snek.h"

Snek::Snek(SnekHead* snekHead)
{
	m_po_Head = snekHead;
	m_i_Player = 0;
}

Snek::~Snek()
{
	m_v_BodyParts.clear();	//calls the destructor for each body part
	delete m_po_Head;		//destroys the head
}

void Snek::AddBodyPart(SnekBody* snekBody)
{
	if (m_v_BodyParts.empty())
		snekBody->m_o_Reference = m_po_Head;
	else
		snekBody->m_o_Reference = m_v_BodyParts.back();
	m_v_BodyParts.push_back(snekBody);
}

void Snek::Update(float dt)
{
	m_po_Head->Update(dt);
	auto i_BodyParts = m_v_BodyParts.begin();
	for (; i_BodyParts != m_v_BodyParts.end(); ++i_BodyParts)
	{
		if(m_po_Head->GetInvulnerable() > 0)
		{
			(*i_BodyParts)->SetColor(9993);
		}else
		{
			(*i_BodyParts)->SetColor(9999);
		}
		(*i_BodyParts)->Update();
	}
	m_po_Head->SetBoostGainRate( 1 / (m_v_BodyParts.size()+1) * 1 + 1 );
}

void Snek::Draw()
{
	auto i_BodyParts = m_v_BodyParts.rbegin();
	for (; i_BodyParts != m_v_BodyParts.rend(); ++i_BodyParts)
	{
		(*i_BodyParts)->Draw();
	}
	m_po_Head->Draw();
}

void Snek::SetPlayer(int i)
{
	m_po_Head->SetPlayer(i);
}
