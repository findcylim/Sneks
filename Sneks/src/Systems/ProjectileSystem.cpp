#include "ProjectileSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/InvulnerableComponent.h"
#include "../Components/DrawComponent.h"
#include "../Components/SnekHeadComponent.h"
#include "../Components/FollowComponent.h"

time_t timeStampProjectile = 0;

ProjectileSystem::ProjectileSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics)
	: BaseSystem(entityManagerPtr)
{
	m_o_GraphicsSystem = graphics;
}

void ProjectileSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

}

void ProjectileSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this, this);
	m_po_EventManagerPtr->AddListener<Events::EV_CREATE_PROJECTILE>(this, this);
}

void ProjectileSystem::Receive(const Events::EV_CREATE_PROJECTILE& eventData)
{
	time_t currTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - timeStampProjectile;
	if (currTime > 3)
	{
		ProjectileEntity* ent = m_po_EntityManager->NewEntity<ProjectileEntity>(kEntityProjectile, eventData.texName);
		auto T_Comp = ent->GetComponent<TransformComponent>();
		T_Comp->SetPositionX(eventData.pos->x);
		T_Comp->SetPositionY(eventData.pos->y);
		T_Comp->SetRotation(eventData.rot);
		T_Comp->m_f_Scale = eventData.scale;

		auto P_Comp = ent->GetComponent<PhysicsComponent>();
		P_Comp->m_x_Velocity.x = eventData.velocity->x;
		P_Comp->m_x_Velocity.y = eventData.velocity->y;
		P_Comp->m_po_TransformComponent = T_Comp;
		P_Comp->m_f_Speed = eventData.speed;

		auto C_Comp = ent->GetComponent<CollisionComponent>();
		C_Comp->m_i_CollisionGroupVec.push_back(kCollGroupMoon);

		auto G_Comp = ent->GetComponent<DrawComponent>();
		m_o_GraphicsSystem->InitializeDrawComponent(G_Comp, eventData.texName);

		timeStampProjectile = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());;
	}
}

void ProjectileSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	if (eventData.object1->m_i_CollisionGroupVec[0] == kCollGroupMoon && eventData.object2->m_i_CollisionGroupVec[0] == kCollGroupSnek2Body)
	{
		auto C_comp = eventData.object1->m_po_OwnerEntity->GetComponent<CollisionComponent>();
		if (C_comp->enabled)
		{
			m_po_EntityManager->AddToDeleteQueue(eventData.object1->m_po_OwnerEntity);
			eventData.object1->m_po_OwnerEntity->GetComponent<CollisionComponent>()->enabled = false;
		}
	}
	m_po_EventManagerPtr->EmitEvent<Events::EV_ENTITY_POOL_CHANGED>(Events::EV_ENTITY_POOL_CHANGED());
}

ProjectileSystem::~ProjectileSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_CREATE_PROJECTILE>(this);
};

//float GetProjectileChargeRate()
//{
//	return timeStampProjectile/3;
//}