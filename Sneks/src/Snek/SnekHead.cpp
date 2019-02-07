#include "SnekHead.h"
#include "../Math/HTVector2.h"

constexpr float kMaxVelocity       = 900;
constexpr float kAccelerationForce = 6;
constexpr float kBrakeForce        = 6;
constexpr float kTurnSpeed         = 6;
constexpr float kFriction          = 1.5f;	   //natural slowdown
constexpr float kTurnMinSpeed      = 60;        //need to be moving at this speed to turn
constexpr float kMinSpeed          = 300;	   //if speed lower than this then clamp to 0
constexpr float kIdleSpeed         = 450;		   //default move speed


float SnekHead::GetInvulnerable()
{
	return m_f_Invulnerable;
}

void SnekHead::SetInvulnerable(float invul)
{
	m_f_Invulnerable = invul;
}

float SnekHead::GetBoost()
{
	return m_f_Boost;
}

void SnekHead::SetBoostGainRate(float boostGainRate)
{
	m_f_BoostGainRate = boostGainRate;
}

void SnekHead::Update(float dt)
{	
	if (GetInvulnerable() > 0)
	{
		SetAlpha(0.33f);
		m_f_Invulnerable -= dt;
	}else
	{
		SetAlpha(1.0f);
	}

	m_f_Boost += m_f_BoostGainRate * dt * 10;

	if (m_f_Boost >= 100.0f)
		m_f_Boost = 100.0f;
	if (m_f_Boost < 0.0f)
		m_f_Boost = 0.0f;
	// for removal
	if (GetAsyncKeyState(m_i_BoostKey) && m_f_Boost > 5)
	{
		SetVelocity(GetVelocity() - kAccelerationForce * 5);
		m_f_Boost -= 35 * dt;
	}

	if (GetAsyncKeyState(m_i_AccelerationKey)) {
		SetVelocity(GetVelocity() - kAccelerationForce);
		//m_px_Particles->SetTexture(m_px_SnekHedBoost);
		//DrawParticles();
	}
	if (GetAsyncKeyState(m_i_BrakeKey) && (GetVelocity() < 0)) {
		SetVelocity(GetVelocity() + kBrakeForce);
		//m_px_Particles->SetTexture(m_px_SnekHedSmoke);
		//DrawParticles();
	}

	if (GetAsyncKeyState(m_i_LeftKey) && (GetVelocity() <= -kTurnMinSpeed)) {
		SetRotation(GetRotation() + kTurnSpeed *dt);
		//m_px_Texture = m_px_SnekHedL;
	}
	else if (GetAsyncKeyState(m_i_RightKey) && (GetVelocity() <= -kTurnMinSpeed)) {
		SetRotation(GetRotation() - kTurnSpeed *dt);
		//m_px_Texture = m_px_SnekHedR;
	}
	else {
		m_px_Texture = m_px_SnekHed;
	}
	//end removal
	if (GetAsyncKeyState(AEVK_0))
	{
		m_f_Scale -= 0.001f;
	}
	else if (GetAsyncKeyState(AEVK_1))
		m_f_Scale += 0.001f;


	//limit max velocity
	if (m_f_Velocity >= kMaxVelocity)
			m_f_Velocity = kMaxVelocity;
	else if (m_f_Velocity <= -kMaxVelocity)
		m_f_Velocity = -kMaxVelocity;

	if (GetAsyncKeyState(m_i_BoostKey) && m_f_Boost > 5)
		m_f_Velocity *= 1.5f;

	////if out of screen, clamp movement
	//if (m_x_Position.x > AEGfxGetWinMaxX() + 2* 1920)// + m_f_SizeX / 2)
	//	m_x_Position.x = AEGfxGetWinMaxX() + 2 * 1920; // +m_f_SizeX / 2;
	//else if (m_x_Position.x < AEGfxGetWinMinX() - 2 * 1920)// - m_f_SizeX / 2)
	//	m_x_Position.x = AEGfxGetWinMinX() - 2 * 1920;// -m_f_SizeX / 2;

	////if out of screen, clamp movement
	//if (m_x_Position.y > AEGfxGetWinMaxY() + 2 * 1080)// + m_f_SizeY / 2)
	//	m_x_Position.y = AEGfxGetWinMaxY() + 2 * 1080;// +m_f_SizeY / 2;
	//else if (m_x_Position.y < AEGfxGetWinMinY() - 2 * 1080)// - m_f_SizeY / 2)
	//	m_x_Position.y = AEGfxGetWinMinY() - 2 * 1080;// -m_f_SizeY / 2;

	/*clamp low velocity to 0 so its not jittery*/
	if (m_f_Velocity >= -kMinSpeed && m_f_Velocity <= kMinSpeed)
		m_f_Velocity = -kIdleSpeed;
	else if (m_f_Velocity < 0)
		m_f_Velocity += kFriction;
	else if (m_f_Velocity > 0)
		m_f_Velocity -= kFriction;

	ApplyVelocity(dt);
}

SnekHead::SnekHead(const float posX, const float posY, const float sizeX, const float sizeY, AEGfxTexture* tex)
	: DrawObject(posX, posY, sizeX, sizeY, tex)
{
	m_px_SnekHed = tex;
	SetScale(0.635f);
}

SnekHead::SnekHead(const float posX, const float posY, const float sizeX, const float sizeY, AEGfxTexture* tex, AEGfxTexture * snekHedL, AEGfxTexture * snekHedR)
	: SnekHead(posX, posY, sizeX, sizeY, tex)
{
	m_px_SnekHed = tex;
	m_px_SnekHedL = snekHedL;
	m_px_SnekHedR = snekHedR;
	SetScale(0.635f);
}

/*
void SnekHead::SetParticles(AEGfxTexture *smoke, AEGfxTexture *boost)
{
	m_px_Particles = new DrawObject(m_x_Position.x, m_x_Position.y, 20, 40, NULL);

	m_px_SnekHedBoost = boost;
	m_px_SnekHedSmoke = smoke;
	m_px_Particles->SetTexture(smoke);
}

void SnekHead::DrawParticles() const
{	
	//m_px_Particles->SetPositionX(GetPosition().x + GetRotatedOffsetXx());
	//m_px_Particles->SetPositionY(GetPosition().y + GetRotatedOffsetXy());
	//m_px_Particles->SetRotation(GetRotation());
	//m_px_Particles->Draw();
}*/

void SnekHead::SetPlayer(int num)
{
	if (num == 1)
	{
		m_i_PlayerNumber = num;
		m_i_AccelerationKey = AEVK_W;
		m_i_BrakeKey = AEVK_S;
		m_i_LeftKey  = AEVK_A;
		m_i_RightKey = AEVK_D;
		m_i_BoostKey = AEVK_SPACE;
	}

}