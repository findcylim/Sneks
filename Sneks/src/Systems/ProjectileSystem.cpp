#include "ProjectileSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/InvulnerableComponent.h"
#include "../Components/DrawComponent.h"
#include "../Components/SnekHeadComponent.h"
#include "../Components/FollowComponent.h"

ProjectileSystem::ProjectileSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics)
	: BaseSystem(entityManagerPtr)
{
	m_o_GraphicsSystem = graphics;
}

void ProjectileSystem::Update(float dt)
{
	bool press = false;

	auto i_SnekHead = static_cast<SnekHeadComponent*>(
		m_po_ComponentManager->GetFirstComponentInstance(kComponentSnekHead));

	auto i_SnekHeadEntity = static_cast<SnekHeadEntity*>(
		m_po_EntityManager->GetFirstEntityInstance(kEntitySnekHead));

	if (GetAsyncKeyState(i_SnekHead->m_i_BoostKey))
	{
		if (!press) {
			auto moonTexture = "Moon";
			CreateMoon(i_SnekHeadEntity, moonTexture);
			press = true;
		}
	}
	else
	{
		press = false;
	}
}

void ProjectileSystem::Initialize()
{
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER_COLLISION>(this);
}

void ProjectileSystem::receive(const Events::Ev_PLAYER_COLLISION& eventData)
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
		(eventData.object1->m_i_CollisionGroupVec[0] == 3))
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
		//BodyInvulnerableSet(snekHeadComponent);
	}
}

ProjectileSystem::~ProjectileSystem()
{
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER_COLLISION>(this);
};

void ProjectileSystem::CreateMoon(SnekHeadEntity* owner, const char* textureName) const
{
	//TODO:: MESH INSTANCING
	//Create a new body part to add to the vector
	auto newMoonEntity = static_cast<MoonEntity*>(
		m_po_EntityManager->NewEntity(kEntityMoon, "Moon"));

	auto ownerTransform = static_cast<TransformComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			owner, kComponentTransform));

	for (auto i_Component : newMoonEntity->m_v_AttachedComponentsList)
	{
		if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			static_cast<TransformComponent*>(i_Component)->SetPosition(
				ownerTransform->m_x_Position.x, ownerTransform->m_x_Position.y);

			static_cast<TransformComponent*>(i_Component)->SetRotation(ownerTransform->GetRotation());
			//TODO: REMOVE HARCCODE
			static_cast<TransformComponent*>(i_Component)->m_f_Scale = 0.635f;

		}
		else if (i_Component->m_x_ComponentID == kComponentDraw)
		{
			static_cast<DrawComponent*>(i_Component)->Initialize(
				m_o_GraphicsSystem->FetchTexture(textureName),
				186, 186, HTColor{ 1,1,1,1 }
			);

		}
		else if (i_Component->m_x_ComponentID == kComponentPhysics)
		{
			static_cast<PhysicsComponent*>(i_Component)->m_f_MaxSpeed = 900;
			static_cast<PhysicsComponent*>(i_Component)->m_f_Speed = 900;
		}
		else if (i_Component->m_x_ComponentID == KComponentInvulnerable)
		{
			static_cast<InvulnerableComponent*>(i_Component)->m_f_InvulnerableTime = 10000;

		}
		else if (i_Component->m_x_ComponentID == kComponentCollision)
		{
			static_cast<CollisionComponent*>(i_Component)->m_i_CollisionGroupVec.push_back
			(11);
		}
	}
}