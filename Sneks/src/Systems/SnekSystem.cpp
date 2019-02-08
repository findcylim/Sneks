#include "SnekSystem.h"
#include "../Components/SnekHeadComponent.h"
#include "../Components/InvulnerableComponent.h"

SnekSystem::SnekSystem(EntityManager* entityManagerPtr) : BaseSystem(entityManagerPtr) {};


void SnekSystem::Update(float dt)
{
	auto i_SnekHead = static_cast<SnekHeadComponent*>(
		m_po_ComponentManager->GetFirstComponentInstance(kComponentSnekHead));

	while (i_SnekHead) {
		auto i_InvulComponent = static_cast<InvulnerableComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				(i_SnekHead), KComponentInvulnerable
			));

		auto i_DrawComponent = static_cast<DrawComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				(i_SnekHead), KComponentInvulnerable
			));

		if (i_InvulComponent->m_f_InvulnerableTime > 0)
		{
			i_DrawComponent->SetAlpha(0.33f);
			i_InvulComponent->m_f_InvulnerableTime -= dt;
		}
		else if (i_DrawComponent->m_f_RgbaColor.alpha != 1.0f)
		{
			i_DrawComponent->SetAlpha(1.0f);
		}

		/*m_f_Boost += m_f_BoostGainRate * dt * 10;

		if (m_f_Boost >= 100.0f)
			m_f_Boost = 100.0f;
		if (m_f_Boost < 0.0f)
			m_f_Boost = 0.0f;*/
		/*
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
			SetRotation(GetRotation() + kTurnSpeed * dt);
			//m_px_Texture = m_px_SnekHedL;
		}
		else if (GetAsyncKeyState(m_i_RightKey) && (GetVelocity() <= -kTurnMinSpeed)) {
			SetRotation(GetRotation() - kTurnSpeed * dt);
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
		if (m_f_Speed >= kMaxVelocity)
			m_f_Speed = kMaxVelocity;
		else if (m_f_Speed <= -kMaxVelocity)
			m_f_Speed = -kMaxVelocity;

		if (GetAsyncKeyState(m_i_BoostKey) && m_f_Boost > 5)
			m_f_Speed *= 1.5f;
			*/


		/*clamp low velocity to 0 so its not jittery*/
		/*if (m_f_Velocity >= -kMinSpeed && m_f_Velocity <= kMinSpeed)
			m_f_Velocity = -kIdleSpeed;
		else if (m_f_Velocity < 0)
			m_f_Velocity += kFriction;
		else if (m_f_Velocity > 0)
			m_f_Velocity -= kFriction;

		ApplyVelocity(dt);*/

		i_SnekHead = static_cast<SnekHeadComponent*>(i_SnekHead->m_po_NextComponent);
	}
}

void SnekSystem::Initialize()
{
}

//HEAD SIZE : 105, 77
//BODY SIZE:  61,  80
//SCALE : 0.635f
void SnekSystem::CreateSnek(SnekHeadEntity* snekHeadEntity,const int numBodyParts)
{
	auto transformComponent = static_cast<TransformComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			snekHeadEntity, kComponentTransform
		));

	for (int i_BodyParts = 0; i_BodyParts < numBodyParts; i_BodyParts++) {
		//Create a new body part to add to the vector
		//TODO AddBodyPart(snekBodyPart);
	}
}

void SnekSystem::FaceReference(const TransformComponent* reference, TransformComponent* toChange) const
{
	float referenceEdgeX = (reference->m_x_Position.x);// +refHead->GetRotatedOffsetXx());
	float referenceEdgeY = (reference->m_x_Position.y);// +refHead->GetRotatedOffsetXy());

	float distanceX = toChange->m_x_Position.x - referenceEdgeX;
	float distanceY = toChange->m_x_Position.y - referenceEdgeY;

	auto newRot = static_cast<float>(atan2(distanceY, distanceX));
	// make sure rotation is between 0 and 2Pi
	if (newRot < 0)
		newRot += 2 * PI;
	else if (newRot >= 2 * PI)
		newRot -= 2 * PI;

	toChange->SetRotation(newRot);

}

void SnekSystem::MoveTowardsReference(const DrawComponent* reference, DrawComponent* toChange) const
{
	FaceReference(reference->m_po_TransformComponent, toChange->m_po_TransformComponent);

	float distanceX = toChange->m_po_TransformComponent->m_x_Position.x 
		- reference->m_po_TransformComponent->m_x_Position.x;
	float distanceY = toChange->m_po_TransformComponent->m_x_Position.y 
		- reference->m_po_TransformComponent->m_x_Position.y;

	float distanceXySquared = distanceX * distanceX + distanceY * distanceY;

	//TODO
	if (fabsf(distanceX) > reference->GetSizeX() * toChange->m_po_TransformComponent->m_f_Scale / 2 || 1) {
		toChange->m_po_TransformComponent->m_x_Position.x = 
			reference->m_po_TransformComponent->m_x_Position.x + distanceX * 0.7f;

		toChange->m_po_TransformComponent->m_x_Position.y = 
			reference->m_po_TransformComponent->m_x_Position.y + distanceY * 0.7f;

	}

	//TODO CHANGE SCALE
	toChange->m_po_TransformComponent->m_f_Scale = reference->m_po_TransformComponent->m_f_Scale;
	/*float distanceXySquared = distanceX * distanceX + distanceY * distanceY;

	//cap max distance for speed calculations at 500
	if (distanceXySquared > 60 * 60)
		distanceXySquared = 60 * 60;
	if (referenceHalfSizeSquared== 0)
		float referenceHalfSizeSquared = (m_o_Reference->GetSizeX() * m_o_Reference->GetScale() / 2) *
													(m_o_Reference->GetSizeX() * m_o_Reference->GetScale() / 2);


	if (distanceXySquared < 30*30)
	{
		SetVelocity(GetVelocity()*0.5f);
	}
	if (distanceXySquared < referenceHalfSizeSquared) {
		SetVelocity(0);
		SetPosition();
	}
	else //move towards the reference
		SetVelocity(-fabsf(-0.0f - ((distanceXySquared - referenceHalfSizeSquared * 0.8f) * 0.002f)));

	*/

	//apply the velocity
	//ApplyVelocity(dt);
	/*AEVec2 directionVector;
	AEVec2FromAngle(&directionVector, m_f_Rotation);

	m_x_Position.x += directionVector.x * m_f_Speed * 300 * dt;
	m_x_Position.y += directionVector.y * m_f_Speed * 300 * dt;*/
}

void SnekSystem::CheckOutOfBounds(TransformComponent* transformComponent) const
{
	//if out of screen, clamp movement
	if (transformComponent->m_x_Position.x > AEGfxGetWinMaxX() + 2 * 1920)// + m_f_SizeX / 2)
		transformComponent->m_x_Position.x = AEGfxGetWinMaxX() + 2 * 1920; // +m_f_SizeX / 2;
	else if (transformComponent->m_x_Position.x < AEGfxGetWinMinX() - 2 * 1920)// - m_f_SizeX / 2)
		transformComponent->m_x_Position.x = AEGfxGetWinMinX() - 2 * 1920;// -m_f_SizeX / 2;

	//if out of screen, clamp movement
	if (transformComponent->m_x_Position.y > AEGfxGetWinMaxY() + 2 * 1080)// + m_f_SizeY / 2)
		transformComponent->m_x_Position.y = AEGfxGetWinMaxY() + 2 * 1080;// +m_f_SizeY / 2;
	else if (transformComponent->m_x_Position.y < AEGfxGetWinMinY() - 2 * 1080)// - m_f_SizeY / 2)
		transformComponent->m_x_Position.y = AEGfxGetWinMinY() - 2 * 1080;// -m_f_SizeY / 2;
}
