#include "SnekHead.h"


#define MAXVELOCITY 5.0f
#define ACCEL_FAC 0.05f
#define BRAKE_FAC 0.05f
#define TURN_SPEED 0.01f
#define FRICTION 0.01f	   //natural slowdown
#define TURN_MINSPEED 0.2f //need to be moving at this speed to turn
#define MINSPEED 0.04f	   //if speed lower than this then clamp to 0
#define IDLESPEED 0.04f	   //default movespeed

void SnekHead::Update()
{


	/*this shouldnt be here, only for testing*/
	if (GetAsyncKeyState(VK_UP)) {
		SetVelocity(GetVelocity() - ACCEL_FAC);
		m_pParticles->SetTexture(m_pSnekHedBoost);
		DrawParticles();
	}
	if (GetAsyncKeyState(VK_DOWN) && (GetVelocity() < 0 )) {
		SetVelocity(GetVelocity() + BRAKE_FAC);
		m_pParticles->SetTexture(m_pSnekHedSmoke);
		DrawParticles();
	}

	if (GetAsyncKeyState(VK_LEFT) && (GetVelocity() <= -TURN_MINSPEED)) {
		SetRotation(GetRotation() + TURN_SPEED);
		m_pTex = m_pSnekHedL;
	} 
	else if (GetAsyncKeyState(VK_RIGHT) && (GetVelocity() <= -TURN_MINSPEED)) {
		SetRotation(GetRotation() - TURN_SPEED);
		m_pTex = m_pSnekHedR;
	}
	else {
		m_pTex = m_pSnekHed;
	}
	/*end removal*/

	//limit max velocity
	if (m_fVelocity >= MAXVELOCITY)
		m_fVelocity = MAXVELOCITY;
	else if (m_fVelocity <= -MAXVELOCITY)
		m_fVelocity = -MAXVELOCITY;
	
	//if out of screen, pop to the other side of screen X
	if (m_fPositionX > AEGfxGetWinMaxX() + m_fSizeX / 2)
		m_fPositionX = AEGfxGetWinMinX() - m_fSizeX / 2;
	else if (m_fPositionX < AEGfxGetWinMinX() - m_fSizeX / 2)
		m_fPositionX = AEGfxGetWinMaxX() + m_fSizeX / 2;

	//if out of screen, pop to the other side of screen Y
	if (m_fPositionY > AEGfxGetWinMaxY() + m_fSizeY / 2)
		m_fPositionY = AEGfxGetWinMinY() - m_fSizeY / 2;
	else if (m_fPositionY < AEGfxGetWinMinY() - m_fSizeY / 2)
		m_fPositionY = AEGfxGetWinMaxY() + m_fSizeY / 2;


	/*clamp low velocity to 0 so its not jittery*/
	if (m_fVelocity >= -MINSPEED && m_fVelocity <= MINSPEED)
		m_fVelocity = -IDLESPEED;
	else if (m_fVelocity < 0)
		m_fVelocity += FRICTION;
	else if (m_fVelocity > 0)
		m_fVelocity -= FRICTION;

	//apply the velocity
	AEVec2 testPos;
	AEVec2FromAngle(&testPos, m_fRotation);

	m_fPositionX += testPos.x * m_fVelocity;
	m_fPositionY += testPos.y * m_fVelocity;

}

SnekHead::SnekHead(float posX, float posY, float sizeX, float sizeY, AEGfxTexture* tex, AEGfxTexture * snekHedL, AEGfxTexture * snekHedR)
: DrawObject(posX, posY, sizeX, sizeY, tex)
{
	m_pSnekHed = tex;
	m_pSnekHedL = snekHedL;
	m_pSnekHedR = snekHedR;

}

void SnekHead::SetParticles(AEGfxTexture *smoke, AEGfxTexture *boost)
{
	m_pParticles = new DrawObject(m_fPositionX, m_fPositionY, 20, 40, NULL);

	m_pSnekHedBoost = boost;
	m_pSnekHedSmoke = smoke;
	m_pParticles->SetTexture(smoke);
}

void SnekHead::DrawParticles() {

	m_pParticles->SetPositionX(GetPositionX() + GetRotatedOffsetXX());
	m_pParticles->SetPositionY(GetPositionY() + GetRotatedOffsetXY());
	m_pParticles->SetRotation(GetRotation());
	m_pParticles->Draw();
}

void SnekHead::Draw() {
	DrawObject::Draw();


}