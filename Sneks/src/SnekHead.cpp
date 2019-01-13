#include "SnekHead.h"

constexpr float kMaxVelocity       = 4.0f;
constexpr float kAccelerationForce = 0.03f;
constexpr float kBrakeForce        = 0.03f;
constexpr float kTurnSpeed         = 0.01f;
constexpr float kFriction          = 0.005f;	   //natural slowdown
constexpr float kTurnMinSpeed      = 0.2f;        //need to be moving at this speed to turn
constexpr float kMinSpeed          = 1.0f;	   //if speed lower than this then clamp to 0
constexpr float kIdleSpeed         = 1.0f;		   //default move speed

void SnekHead::Update()
{	
	/*this shouldn't be here, only for testing*/
	if (GetAsyncKeyState(m_i_AccelerationKey)) {
		SetVelocity(GetVelocity() - kAccelerationForce);
		m_px_Particles->SetTexture(m_px_SnekHedBoost);
		DrawParticles();
	}
	if (GetAsyncKeyState(m_i_BrakeKey) && (GetVelocity() < 0)) {
		SetVelocity(GetVelocity() + kBrakeForce);
		m_px_Particles->SetTexture(m_px_SnekHedSmoke);
		DrawParticles();
	}

	if (GetAsyncKeyState(m_i_LeftKey) && (GetVelocity() <= -kTurnMinSpeed)) {
		SetRotation(GetRotation() + kTurnSpeed);
		//m_px_Texture = m_px_SnekHedL;
	}
	else if (GetAsyncKeyState(m_i_RightKey) && (GetVelocity() <= -kTurnMinSpeed)) {
		SetRotation(GetRotation() - kTurnSpeed);
		//m_px_Texture = m_px_SnekHedR;
	}
	else {
		m_px_Texture = m_px_SnekHed;
	}
	/*end removal*/

	//limit max velocity
	if (m_f_Velocity >= kMaxVelocity)
		m_f_Velocity = kMaxVelocity;
	else if (m_f_Velocity <= -kMaxVelocity)
		m_f_Velocity = -kMaxVelocity;
	/*
	//if out of screen, pop to the other side of screen X
	if (m_f_PositionX > AEGfxGetWinMaxX() + m_f_SizeX / 2)
		m_f_PositionX = AEGfxGetWinMinX() - m_f_SizeX / 2;
	else if (m_f_PositionX < AEGfxGetWinMinX() - m_f_SizeX / 2)
		m_f_PositionX = AEGfxGetWinMaxX() + m_f_SizeX / 2;

	//if out of screen, pop to the other side of screen Y
	if (m_f_PositionY > AEGfxGetWinMaxY() + m_f_SizeY / 2)
		m_f_PositionY = AEGfxGetWinMinY() - m_f_SizeY / 2;
	else if (m_f_PositionY < AEGfxGetWinMinY() - m_f_SizeY / 2)
		m_f_PositionY = AEGfxGetWinMaxY() + m_f_SizeY / 2;
		*/

		//if out of screen, clamp movement
	if (m_f_PositionX > AEGfxGetWinMaxX() + m_f_SizeX / 2)
		m_f_PositionX = AEGfxGetWinMaxX() + m_f_SizeX / 2;
	else if (m_f_PositionX < AEGfxGetWinMinX() - m_f_SizeX / 2)
		m_f_PositionX = AEGfxGetWinMinX() - m_f_SizeX / 2;

	//if out of screen, clamp movement
	if (m_f_PositionY > AEGfxGetWinMaxY() + m_f_SizeY / 2)
		m_f_PositionY = AEGfxGetWinMaxY() + m_f_SizeY / 2;
	else if (m_f_PositionY < AEGfxGetWinMinY() - m_f_SizeY / 2)
		m_f_PositionY = AEGfxGetWinMinY() - m_f_SizeY / 2;

	/*clamp low velocity to 0 so its not jittery*/
	if (m_f_Velocity >= -kMinSpeed && m_f_Velocity <= kMinSpeed)
		m_f_Velocity = -kIdleSpeed;
	else if (m_f_Velocity < 0)
		m_f_Velocity += kFriction;
	else if (m_f_Velocity > 0)
		m_f_Velocity -= kFriction;

	//apply the velocity
	AEVec2 testPos;
	AEVec2FromAngle(&testPos, m_f_Rotation);

	m_f_PositionX += testPos.x * m_f_Velocity;
	m_f_PositionY += testPos.y * m_f_Velocity;
}

SnekHead::SnekHead(const float posX, const float posY, const float sizeX, const float sizeY, AEGfxTexture* tex)
	: DrawObject(posX, posY, sizeX, sizeY, tex)
{
	m_px_SnekHed = tex;
}

SnekHead::SnekHead(const float posX, const float posY, const float sizeX, const float sizeY, AEGfxTexture* tex, AEGfxTexture * snekHedL, AEGfxTexture * snekHedR)
	: SnekHead(posX, posY, sizeX, sizeY, tex)
{
	m_px_SnekHed = tex;
	m_px_SnekHedL = snekHedL;
	m_px_SnekHedR = snekHedR;
}

void SnekHead::SetParticles(AEGfxTexture *smoke, AEGfxTexture *boost)
{
	m_px_Particles = new DrawObject(m_f_PositionX, m_f_PositionY, 20, 40, NULL);

	m_px_SnekHedBoost = boost;
	m_px_SnekHedSmoke = smoke;
	m_px_Particles->SetTexture(smoke);
}

void SnekHead::DrawParticles() const
{
	m_px_Particles->SetPositionX(GetPositionX() + GetRotatedOffsetXx());
	m_px_Particles->SetPositionY(GetPositionY() + GetRotatedOffsetXy());
	m_px_Particles->SetRotation(GetRotation());
	m_px_Particles->Draw();
}

void SnekHead::SetPlayer(int num)
{
	if (num == 1)
	{
		m_i_PlayerNumber = num;
		m_i_AccelerationKey = AEVK_W;
		m_i_BrakeKey = AEVK_S;
		m_i_LeftKey  = AEVK_A;
		m_i_RightKey = AEVK_D;
	}

}