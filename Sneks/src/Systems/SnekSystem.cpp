#include "SnekSystem.h"
#include "../Components/SnekHeadComponent.h"
#include "../Components/InvulnerableComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/FollowComponent.h"
#include <iostream>

SnekSystem::SnekSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics)
: BaseSystem(entityManagerPtr)
{
	m_o_GraphicsSystem = graphics;
}

SnekSystem::~SnekSystem()
{
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER_COLLISION>(this);
};


void SnekSystem::receive(const Events::Ev_PLAYER_COLLISION& eventData)
{
	//if its a building
	if (eventData.object1->m_i_CollisionGroupVec[0] == 10)
	{
		eventData.object1->enabled = false;
		auto objectDrawComp = static_cast<DrawComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				eventData.object1, kComponentDraw
			));
		objectDrawComp->m_px_Texture = m_o_GraphicsSystem->FetchTexture("Destroyed01");
	}
	if (eventData.object2->m_i_CollisionGroupVec[0] == 10)
	{
		eventData.object2->enabled = false;
		auto objectDrawComp = static_cast<DrawComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				eventData.object2, kComponentDraw
			));
		objectDrawComp->m_px_Texture = m_o_GraphicsSystem->FetchTexture("Destroyed01");
	}
	//body collision
	if (eventData.object1->m_i_CollisionGroupVec[0] == 1 ||
		(eventData.object1->m_i_CollisionGroupVec[0] == 3) )
	{
		//Get the parent
		auto objectFollowComp = static_cast<FollowComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				eventData.object1, kComponentFollow
			));

		auto snekHeadComponent = static_cast<SnekHeadComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				objectFollowComp->m_po_ParentEntity, kComponentSnekHead
			));

		//m_po_EntityManager->DeleteEntity(snekHeadComponent->m_x_BodyParts.back());
		snekHeadComponent->m_x_BodyParts.pop_back();


		auto snakeHeadInvulComponent = static_cast<InvulnerableComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				objectFollowComp->m_po_ParentEntity, KComponentInvulnerable
			));

		snakeHeadInvulComponent->m_f_InvulnerableTime = 3.0f;
		
	}
	if (eventData.object2->m_i_CollisionGroupVec[0] == 1 ||
		(eventData.object2->m_i_CollisionGroupVec[0] == 3))
	{
		//Get the parent
		auto objectFollowComp = static_cast<FollowComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				eventData.object2, kComponentFollow
			));

		auto snekHeadComponent = static_cast<SnekHeadComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				objectFollowComp->m_po_ParentEntity, kComponentSnekHead
			));

		//m_po_EntityManager->DeleteEntity(snekHeadComponent->m_x_BodyParts.back());
		snekHeadComponent->m_x_BodyParts.pop_back();

		auto snakeHeadInvulComponent = static_cast<InvulnerableComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				objectFollowComp->m_po_ParentEntity, KComponentInvulnerable
			));

		snakeHeadInvulComponent->m_f_InvulnerableTime = 3.0f;
		BodyInvulnerableSet(snekHeadComponent);

	}

	/*std::cout << "Colliding: " << eventData.object1->m_po_OwnerEntity->m_pc_EntityName << " and " <<
		eventData.object2->m_po_OwnerEntity->m_pc_EntityName << std::endl;*/
}

void SnekSystem::receive(const Events::Ev_SNEK_INVULNERABLE& eventData)
{
	BodyInvulnerableSet(eventData.snekHead);
}

void SnekSystem::Update(float dt)
{
	auto i_InvulnerableComponent = static_cast<InvulnerableComponent*>(
		m_po_ComponentManager->GetFirstComponentInstance(KComponentInvulnerable));

	while (i_InvulnerableComponent)
	{
		CheckInvulnerability(i_InvulnerableComponent, dt);
		i_InvulnerableComponent = static_cast<InvulnerableComponent*>(
			i_InvulnerableComponent->m_po_NextComponent
			);
	}
	auto i_SnekHeadEntity = static_cast<SnekHeadEntity*>(
		m_po_EntityManager->GetFirstEntityInstance(kEntitySnekHead));

	auto i_SnekHead = static_cast<SnekHeadComponent*>(
		m_po_ComponentManager->GetFirstComponentInstance(kComponentSnekHead));

	while (i_SnekHead) {
		auto headInvulComponent = static_cast<InvulnerableComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				(i_SnekHead->m_po_OwnerEntity), KComponentInvulnerable
			));

		auto headPhysicsComponent = static_cast<PhysicsComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				i_SnekHead->m_po_OwnerEntity, kComponentPhysics));

		
		if (GetAsyncKeyState(i_SnekHead->m_i_AccelerationKey)) 
		{
			Events::Ev_PLAYER_MOVEMENTKEY moveKey{ headPhysicsComponent, Events::MOVEKEY_UP};
			m_o_EventManagerPtr->EmitEvent<Events::Ev_PLAYER_MOVEMENTKEY>(moveKey);
		}
		else
		{
			headPhysicsComponent->m_f_Acceleration = 0;
		}
		if (GetAsyncKeyState(i_SnekHead->m_i_LeftKey))
		{
			Events::Ev_PLAYER_MOVEMENTKEY moveKey{ headPhysicsComponent, Events::MOVEKEY_LEFT };
			m_o_EventManagerPtr->EmitEvent<Events::Ev_PLAYER_MOVEMENTKEY>(moveKey);
		}
		else if (GetAsyncKeyState(i_SnekHead->m_i_RightKey))
		{
			Events::Ev_PLAYER_MOVEMENTKEY moveKey{ headPhysicsComponent,Events::MOVEKEY_RIGHT };
			m_o_EventManagerPtr->EmitEvent<Events::Ev_PLAYER_MOVEMENTKEY>(moveKey);
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
			if (!GetAsyncKeyState(AEVK_0))
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

void SnekSystem::CheckInvulnerability(BaseComponent* component, float dt) const
{
	auto invulComponent = static_cast<InvulnerableComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
		component, KComponentInvulnerable
		));

	auto drawComponent = static_cast<DrawComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
		invulComponent, kComponentDraw
		));


	auto collisionComponent = static_cast<CollisionComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
		invulComponent, kComponentCollision
		));

	if (invulComponent->m_f_InvulnerableTime > 0)
	{
		drawComponent->SetAlpha(0.33f);
		collisionComponent->enabled = false;
		invulComponent->m_f_InvulnerableTime -= dt;
	}
	else if (drawComponent->m_f_RgbaColor.alpha != 1.0f)
	{
		collisionComponent->enabled = true;
		drawComponent->SetAlpha(1.0f);
	}
}

void SnekSystem::BodyInvulnerableSet(SnekHeadComponent* snekHead) const
{
	auto headInvulComponent = static_cast<InvulnerableComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
		snekHead, KComponentInvulnerable
		));

	for (auto i_BodyParts : snekHead->m_x_BodyParts)
	{
		auto invulComponent = static_cast<InvulnerableComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				i_BodyParts, KComponentInvulnerable
			));
		invulComponent->m_f_InvulnerableTime = headInvulComponent->m_f_InvulnerableTime;
	}
}




void SnekSystem::Initialize()
{
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER_COLLISION>(this);
	m_o_EventManagerPtr->AddListener<Events::Ev_SNEK_INVULNERABLE>(this);
}

//HEAD SIZE : 105, 77
//BODY SIZE:  61,  80
//SCALE : 0.635f
void SnekSystem::CreateSnek(float posX, float posY, float rotation,
	const int numBodyParts, const char* textureName, int controlScheme) const
{
	//Count the number of previous snek heads
	int snekHeadCount = 0;
	auto i_SnekHeadComponents = static_cast<SnekHeadComponent*>(
		m_po_ComponentManager->GetFirstComponentInstance(
			kComponentSnekHead
		));
	while (i_SnekHeadComponents)
	{
		snekHeadCount++;
		i_SnekHeadComponents = static_cast<SnekHeadComponent*>(i_SnekHeadComponents->m_po_NextComponent);
	}

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
			if (snekHeadCount == 0)
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
				(snekHeadCount * 2);
		}
	}

	auto bodyTexture = "SnekBody01";
	if (!strcmp(textureName, "SnekHead02"))
	{
		bodyTexture = "snake-body2.png";
	}

	for (int i_BodyParts = 0; i_BodyParts < numBodyParts; i_BodyParts++){
		CreateSnekBody(newSnekHeadEntity, bodyTexture, snekHeadCount);
	}

	auto tailTexture = "snake-tail2.png";
	CreateSnekTail(newSnekHeadEntity, tailTexture);
}

void SnekSystem::CreateSnekBody(SnekHeadEntity* owner, const char* textureName, int playerNumber) const 
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
			srand(clock());
			int i = rand() % 10;
			static_cast<DrawComponent*>(i_Component)->Initialize(
				m_o_GraphicsSystem->FetchTexture(textureName),
				61, 80, HTColor{ i * 0.1f,i * 0.1f,1,1 }
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
			(playerNumber * 2 + 1);
		}
	}

	auto ownerHeadComponent = static_cast<SnekHeadComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
		owner, kComponentSnekHead));

	auto followComponent = static_cast<FollowComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			newSnekBodyEntity, kComponentFollow));

	followComponent->m_po_ParentEntity = owner;

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
			snekHeadComponent->m_x_BodyParts.back(), kComponentTransform
		));

	auto tempX = headTransformComponent->GetPosition().x;
	auto tempY = headTransformComponent->GetPosition().y;
	headTransformComponent->SetPosition(tailTransformComponent->GetPosition().x, tailTransformComponent->GetPosition().y);
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

	if (numBodyParts%2)
	for (int i_BodyPartsFront = 0, i_BodyPartsBack = (numBodyParts - 1); i_BodyPartsFront == i_BodyPartsBack; i_BodyPartsFront++, i_BodyPartsBack--) {

		auto frontTransformComponent = static_cast<TransformComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				snekHeadComponent->m_x_BodyParts[i_BodyPartsFront], kComponentTransform
			));

		auto backTransformComponent = static_cast<TransformComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				snekHeadComponent->m_x_BodyParts[i_BodyPartsBack], kComponentTransform
			));

		auto tempX = frontTransformComponent->GetPosition().x;
		auto tempY = frontTransformComponent->GetPosition().y;
		frontTransformComponent->SetPosition(backTransformComponent->GetPosition().x, backTransformComponent->GetPosition().y);
		backTransformComponent->SetPosition(tempX, tempY);
	}

	else
	for (int i_BodyPartsFront = 0, i_BodyPartsBack = (numBodyParts - 1); i_BodyPartsFront == i_BodyPartsBack - 1; i_BodyPartsFront++, i_BodyPartsBack--) {

		auto frontTransformComponent = static_cast<TransformComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				snekHeadComponent->m_x_BodyParts[i_BodyPartsFront], kComponentTransform
			));

		auto backTransformComponent = static_cast<TransformComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				snekHeadComponent->m_x_BodyParts[i_BodyPartsBack], kComponentTransform
			));

		auto tempX = frontTransformComponent->GetPosition().x;
		auto tempY = frontTransformComponent->GetPosition().y;
		frontTransformComponent->SetPosition(backTransformComponent->GetPosition().x, backTransformComponent->GetPosition().y);
		backTransformComponent->SetPosition(tempX, tempY);
	}

}

