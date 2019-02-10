#include "SnekSystem.h"
#include "../Components/SnekHeadComponent.h"
#include "../Components/InvulnerableComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/FollowComponent.h"

SnekSystem::SnekSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics)
: BaseSystem(entityManagerPtr)
{
	m_o_GraphicsSystem = graphics;
};


void SnekSystem::Update(float dt)
{
	auto i_SnekHeadEntity = static_cast<SnekHeadEntity*>(
		m_po_EntityManager->GetFirstEntityInstance(kEntitySnekHead));

	auto i_SnekHead = static_cast<SnekHeadComponent*>(
		m_po_ComponentManager->GetFirstComponentInstance(kComponentSnekHead));

	while (i_SnekHead) {
		auto headInvulComponent = static_cast<InvulnerableComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				(i_SnekHead->m_po_OwnerEntity), KComponentInvulnerable
			));

		auto headDrawComponent = static_cast<DrawComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				(i_SnekHead->m_po_OwnerEntity), kComponentDraw
			));

		if (headInvulComponent->m_f_InvulnerableTime > 0)
		{
			headDrawComponent->SetAlpha(0.33f);
			headInvulComponent->m_f_InvulnerableTime -= dt;
		}
		else if (headDrawComponent->m_f_RgbaColor.alpha != 1.0f)
		{
			headDrawComponent->SetAlpha(1.0f);
		}

		auto headPhysicsComponent = static_cast<PhysicsComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				i_SnekHead->m_po_OwnerEntity, kComponentPhysics));

		if (GetAsyncKeyState(i_SnekHead->m_i_BoostKey))
			Flip(i_SnekHeadEntity);

		if (GetAsyncKeyState(i_SnekHead->m_i_AccelerationKey)) {
			headPhysicsComponent->m_f_Acceleration = i_SnekHead->m_f_AccelerationForce;
			//SetVelocity(GetVelocity() - kAccelerationForce);
		}else
		{
			headPhysicsComponent->m_f_Acceleration = 0;
		}
		if (GetAsyncKeyState(i_SnekHead->m_i_LeftKey))
		{
			headPhysicsComponent->m_po_TransformComponent->SetRotation(
				headPhysicsComponent->m_po_TransformComponent->GetRotation() +
				i_SnekHead->m_f_TurnSpeed * dt
			);
		}
		else if (GetAsyncKeyState(i_SnekHead->m_i_RightKey))
		{
			headPhysicsComponent->m_po_TransformComponent->SetRotation(
				headPhysicsComponent->m_po_TransformComponent->GetRotation() -
				i_SnekHead->m_f_TurnSpeed * dt
			);
		}
		for (size_t i_Body = 0; i_Body < i_SnekHead->m_x_BodyParts.size(); i_Body++)
		{
			
		}
		for (auto i_Body : i_SnekHead->m_x_BodyParts)
		{
			auto bodyDraw = static_cast<DrawComponent*>(
				m_po_ComponentManager->GetSpecificComponentInstance(
					i_Body, kComponentDraw
				));

			auto followComponent = static_cast<FollowComponent*>(
				m_po_ComponentManager->GetSpecificComponentInstance(
					i_Body, kComponentFollow
				));

			auto followDrawComponent = static_cast<DrawComponent*>(
				m_po_ComponentManager->GetSpecificComponentInstance(
					followComponent->m_po_TransformComponent->m_po_OwnerEntity, kComponentDraw
				));
			
			FaceReference(followComponent->m_po_TransformComponent, bodyDraw->m_po_TransformComponent);
			MoveTowardsReference(followDrawComponent, bodyDraw, headPhysicsComponent);
		}
		/*
		else if (GetAsyncKeyState(i_SnekHead->m_i_BrakeKey) && (GetVelocity() < 0)) {
			SetVelocity(GetVelocity() + kBrakeForce);
			//m_px_Particles->SetTexture(m_px_SnekHedSmoke);
			//DrawParticles();
		}

		if (GetAsyncKeyState(i_SnekHead->m_i_LeftKey) && (GetVelocity() <= -kTurnMinSpeed)) {
			SetRotation(GetRotation() + kTurnSpeed * dt);
			//m_px_Texture = m_px_SnekHedL;
		}
		else if (GetAsyncKeyState(i_SnekHead->m_i_RightKey) && (GetVelocity() <= -kTurnMinSpeed)) {
			SetRotation(GetRotation() - kTurnSpeed * dt);
			//m_px_Texture = m_px_SnekHedR;
		}*/

		//TODO
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
void SnekSystem::CreateSnek(float posX, float posY, float rotation,
	const int numBodyParts, const char* textureName, int controlScheme) const
{

	auto newSnekHeadEntity = static_cast<SnekHeadEntity*>(
		m_po_EntityManager->NewEntity(kEntitySnekHead, "Head"));

	for (auto i_Component : newSnekHeadEntity->m_v_AttachedComponentsList)
	{
		if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			static_cast<TransformComponent*>(i_Component)->SetPosition(posX, posY);
			static_cast<TransformComponent*>(i_Component)->SetRotation(rotation);

			auto cameraComponent = static_cast<CameraComponent*>(
				m_po_ComponentManager->GetFirstComponentInstance(
					kComponentCamera
				));
			cameraComponent->AddToTrack(static_cast<TransformComponent*>(i_Component));
			static_cast<TransformComponent*>(i_Component)->m_f_Scale = 0.635f;

		}
		else if(i_Component->m_x_ComponentID == kComponentDraw)
		{
			static_cast<DrawComponent*>(i_Component)->Initialize(
				m_o_GraphicsSystem->FetchTexture(textureName),
				105, 77, HTColor{ 1,1,1,1 }
			);
			static_cast<DrawComponent*>(i_Component)->m_f_DrawPriority = 4;
		}
		else if (i_Component->m_x_ComponentID == kComponentPhysics)
		{
			static_cast<PhysicsComponent*>(i_Component)->m_f_MaxSpeed = 900;
		}
		else if (i_Component->m_x_ComponentID == kComponentSnekHead)
		{
			if (controlScheme == 0)
			{
				
			}else
			{//TODO:: REMOVE HARDCODE
				static_cast<SnekHeadComponent*>(i_Component)->m_i_AccelerationKey = AEVK_W;
				static_cast<SnekHeadComponent*>(i_Component)->m_i_BrakeKey = AEVK_S;
				static_cast<SnekHeadComponent*>(i_Component)->m_i_LeftKey = AEVK_A;
				static_cast<SnekHeadComponent*>(i_Component)->m_i_RightKey = AEVK_D;
			}
			//TODO :: LOTS OF SHIT
			//((SnekHeadComponent*)i_Component)->
		}
		else if (i_Component->m_x_ComponentID == KComponentInvulnerable)
		{
			static_cast<InvulnerableComponent*>(i_Component)->m_f_InvulnerableTime = 0;

		}
		else if (i_Component->m_x_ComponentID == kComponentCollision)
		{
			static_cast<CollisionComponent*>(i_Component)->m_i_CollisionGroupVec.push_back
				(1);
		}
	}

	auto bodyTexture = "SnekBody01";
	if (!strcmp(textureName, "SnekHead02"))
	{
		bodyTexture = "snake-body2.png";
	}

	for (int i_BodyParts = 0; i_BodyParts < numBodyParts; i_BodyParts++){
		CreateSnekBody(newSnekHeadEntity, bodyTexture);
	}

	auto tailTexture = "snake-tail2.png";
	CreateSnekTail(newSnekHeadEntity, tailTexture);
}

void SnekSystem::CreateSnekBody(SnekHeadEntity* owner, const char* textureName) const 
{
	//TODO:: MESH INSTANCING
	//Create a new body part to add to the vector
	auto newSnekBodyEntity = static_cast<SnekBodyEntity*>(
		m_po_EntityManager->NewEntity(kEntitySnekBody, "Body"));

	auto ownerTransform = static_cast<TransformComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
		owner, kComponentTransform));

	for (auto i_Component : newSnekBodyEntity->m_v_AttachedComponentsList)
	{
		if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			static_cast<TransformComponent*>(i_Component)->SetPosition(
				ownerTransform->m_x_Position.x, ownerTransform->m_x_Position.y);

			static_cast<TransformComponent*>(i_Component)->SetRotation(0);
			//TODO: REMOVE HARCCODE
			static_cast<TransformComponent*>(i_Component)->m_f_Scale = 0.635f;

		}
		else if (i_Component->m_x_ComponentID == kComponentDraw)
		{
			static_cast<DrawComponent*>(i_Component)->Initialize(
				m_o_GraphicsSystem->FetchTexture(textureName),
				61, 80, HTColor{ 1,1,1,1 }
			);
			
		}
		else if (i_Component->m_x_ComponentID == kComponentPhysics)
		{
			static_cast<PhysicsComponent*>(i_Component)->m_f_MaxSpeed = 900;
		}
		else if (i_Component->m_x_ComponentID == KComponentInvulnerable)
		{
			static_cast<InvulnerableComponent*>(i_Component)->m_f_InvulnerableTime = 0;

		}
		else if (i_Component->m_x_ComponentID == kComponentCollision)
		{
			static_cast<CollisionComponent*>(i_Component)->m_i_CollisionGroupVec.push_back
			(2);
		}
	}

	auto ownerHeadComponent = static_cast<SnekHeadComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
		owner, kComponentSnekHead));

	auto followComponent = static_cast<FollowComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			newSnekBodyEntity, kComponentFollow));

	if (ownerHeadComponent->m_x_BodyParts.empty())
	{
		followComponent->m_po_TransformComponent = ownerTransform;
	}
	else
	{
		auto toFollowTransform = static_cast<TransformComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				ownerHeadComponent->m_x_BodyParts.back(), kComponentTransform));

		followComponent->m_po_TransformComponent = toFollowTransform;
	}
	ownerHeadComponent->m_x_BodyParts.push_back(newSnekBodyEntity);

}

void SnekSystem::CreateSnekTail(SnekHeadEntity* owner, const char* textureName) const
{
	//TODO:: MESH INSTANCING
	//Create a new body part to add to the vector
	auto newSnekBodyEntity = static_cast<SnekBodyEntity*>(
		m_po_EntityManager->NewEntity(kEntitySnekBody, "Body"));

	auto ownerTransform = static_cast<TransformComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			owner, kComponentTransform));

	for (auto i_Component : newSnekBodyEntity->m_v_AttachedComponentsList)
	{
		if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			static_cast<TransformComponent*>(i_Component)->SetPosition(
				ownerTransform->m_x_Position.x, ownerTransform->m_x_Position.y);

			static_cast<TransformComponent*>(i_Component)->SetRotation(0);
			//TODO: REMOVE HARCCODE
			static_cast<TransformComponent*>(i_Component)->m_f_Scale = 0.635f;

		}
		else if (i_Component->m_x_ComponentID == kComponentDraw)
		{
			static_cast<DrawComponent*>(i_Component)->Initialize(
				m_o_GraphicsSystem->FetchTexture(textureName),
				105, 77, HTColor{ 1,1,1,1 }
			);

		}
		else if (i_Component->m_x_ComponentID == kComponentPhysics)
		{
			static_cast<PhysicsComponent*>(i_Component)->m_f_MaxSpeed = 900;
		}
		else if (i_Component->m_x_ComponentID == KComponentInvulnerable)
		{
			static_cast<InvulnerableComponent*>(i_Component)->m_f_InvulnerableTime = 0;

		}
		else if (i_Component->m_x_ComponentID == kComponentCollision)
		{
			static_cast<CollisionComponent*>(i_Component)->m_i_CollisionGroupVec.push_back
			(2);
		}
	}

	auto ownerHeadComponent = static_cast<SnekHeadComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			owner, kComponentSnekHead));

	auto followComponent = static_cast<FollowComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			newSnekBodyEntity, kComponentFollow));

	if (ownerHeadComponent->m_x_BodyParts.empty())
	{
		followComponent->m_po_TransformComponent = ownerTransform;
	}
	else
	{
		auto toFollowTransform = static_cast<TransformComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				ownerHeadComponent->m_x_BodyParts.back(), kComponentTransform));

		followComponent->m_po_TransformComponent = toFollowTransform;
	}
	ownerHeadComponent->m_x_BodyParts.push_back(newSnekBodyEntity);

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
	toChange->m_f_Scale = reference->m_f_Scale;

}

void SnekSystem::MoveTowardsReference(DrawComponent* reference, DrawComponent* toChange, PhysicsComponent* headPhysicsComponent) const
{

	float distanceX = toChange->m_po_TransformComponent->m_x_Position.x -
		 reference->m_po_TransformComponent->m_x_Position.x;
	float distanceY = toChange->m_po_TransformComponent->m_x_Position.y -
		 reference->m_po_TransformComponent->m_x_Position.y;

	float distanceXySquared = distanceX * distanceX + distanceY * distanceY;

	//TODO
	if (fabsf(distanceX) > reference->GetSizeX() * toChange->m_po_TransformComponent->m_f_Scale / 2 || 1) {
		toChange->m_po_TransformComponent->m_x_Position.x =
			reference->m_po_TransformComponent->m_x_Position.x + distanceX
			* (0.95f - 0.5f * (headPhysicsComponent->m_f_Speed / headPhysicsComponent->m_f_MaxSpeed));

		toChange->m_po_TransformComponent->m_x_Position.y =
			reference->m_po_TransformComponent->m_x_Position.y + distanceY
			* (0.95f - 0.5f * (headPhysicsComponent->m_f_Speed / headPhysicsComponent->m_f_MaxSpeed));

	}

}

void SnekSystem::Flip(SnekHeadEntity* owner)
{

	auto snekPhysicsComponent = static_cast<PhysicsComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			owner, kComponentPhysics
		));

	snekPhysicsComponent->m_f_Acceleration = 0;

	auto snekHeadComponent = static_cast<SnekHeadComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			owner, kComponentSnekHead
		));

	int numBodyParts = snekHeadComponent->m_x_BodyParts.size() - 1;

	auto headTransformComponent = static_cast<TransformComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			owner, kComponentTransform
		));

	auto tailTransformComponent = static_cast<TransformComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			snekHeadComponent->m_x_BodyParts[numBodyParts], kComponentTransform
		));

	auto tempX = headTransformComponent->GetPositionX();
	auto tempY = headTransformComponent->GetPositionY();
	headTransformComponent->SetPosition(tailTransformComponent->GetPositionX(), tailTransformComponent->GetPositionY());
	tailTransformComponent->SetPosition(tempX, tempY);

	tempX = headTransformComponent->GetRotation();
	headTransformComponent->SetRotation(tailTransformComponent->GetRotation());
	tailTransformComponent->SetRotation(tempX);

	FollowComponent* bodyFollowComponent;

	for (int i_BodyPartsFront = 0; i_BodyPartsFront < numBodyParts; i_BodyPartsFront++) {
		bodyFollowComponent = static_cast<FollowComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				snekHeadComponent->m_x_BodyParts[i_BodyPartsFront], kComponentFollow
			));

		auto nextbodyTransformComponent = static_cast<TransformComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				snekHeadComponent->m_x_BodyParts[i_BodyPartsFront + 1], kComponentTransform
			));

		bodyFollowComponent->m_po_TransformComponent = nextbodyTransformComponent;
	}

	bodyFollowComponent = static_cast<FollowComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			snekHeadComponent->m_x_BodyParts[numBodyParts - 1], kComponentFollow
		));

	bodyFollowComponent->m_po_TransformComponent = headTransformComponent;

	auto bodyTransformComponent = static_cast<TransformComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			snekHeadComponent->m_x_BodyParts[0], kComponentTransform
		));

	auto tailFollowComponent = static_cast<FollowComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			snekHeadComponent->m_x_BodyParts[numBodyParts], kComponentFollow
		));

	tailFollowComponent->m_po_TransformComponent = bodyTransformComponent;

	for (int i_BodyPartsFront = 0, i_BodyPartsBack = (numBodyParts - 1); i_BodyPartsFront == i_BodyPartsBack; i_BodyPartsFront++, i_BodyPartsBack--) {

		auto frontTransformComponent = static_cast<TransformComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				snekHeadComponent->m_x_BodyParts[i_BodyPartsFront], kComponentTransform
			));

		auto backTransformComponent = static_cast<TransformComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				snekHeadComponent->m_x_BodyParts[i_BodyPartsBack], kComponentTransform
			));

		auto tempX = frontTransformComponent->GetPositionX();
		auto tempY = frontTransformComponent->GetPositionY();
		frontTransformComponent->SetPosition(backTransformComponent->GetPositionX(), backTransformComponent->GetPositionY());
		backTransformComponent->SetPosition(tempX, tempY);
	}

}

