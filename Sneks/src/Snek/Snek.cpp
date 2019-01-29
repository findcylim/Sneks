#include "Snek.h"

Snek::Snek(SnekHead* snekHead, SnekBody* snekBody)
{
	m_po_Head = snekHead;
	m_i_Player = 0;
}

Snek::Snek(const int numBodyParts, float posX, float posY, AEGfxTexture* snakeHeadTexture, AEGfxTexture* snakeBodyTexture)
{
	//Create head mesh based on Texture
	m_po_Head = static_cast<SnekHead*>(new SnekHead(posX, posY, 105, 77, snakeHeadTexture));
	m_po_Head->SetRotation(PI);

	//camera->AddToTrack(snekHeadTest);
	for (int i_BodyParts = 0; i_BodyParts < numBodyParts; i_BodyParts++) {
		//TODO: SET SIZEx AND SIZEy to auto detected TEXTURE SIZE values
		//Create a new body part to add to the vector
		auto snekBodyPart = static_cast<SnekBody*>(new DrawObject(posX, posY, 61, 80, snakeBodyTexture));
		AddBodyPart(snekBodyPart);
	}
	m_i_Player = 0;
}

Snek::~Snek()
{
	m_v_BodyParts.clear();	//calls the destructor for each body part
	//TODO: DELETE BODY PARTS PROPERLY
	delete m_po_Head;			//destroys the head
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
			(*i_BodyParts)->SetAlpha(0.33f);
		}else
		{
			(*i_BodyParts)->SetAlpha(1.0f);
		}
		(*i_BodyParts)->Update(dt);
	}
	m_po_Head->SetBoostGainRate( 1.0f / (m_v_BodyParts.size()+1) * 1.0f + 1.0f );
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

void Snek::SetPlayer(int i) const
{
	m_po_Head->SetPlayer(i);
}
