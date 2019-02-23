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
	UNREFERENCED_PARAMETER(dt);

}

void ProjectileSystem::Initialize()
{
	m_o_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this);
	m_o_EventManagerPtr->AddListener<Events::EV_CREATE_PROJECTILE>(this);
}

void ProjectileSystem::Receive(const Events::EV_CREATE_PROJECTILE& eventData)
{
	ProjectileEntity* ent = static_cast<ProjectileEntity*>(m_po_EntityManager->NewEntity(kEntityProjectile, eventData.texName));
	auto T_Comp = ent->GetComponent<TransformComponent>();
	T_Comp->SetPosition(eventData.pos->x, eventData.pos->y);
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
	G_Comp->Initialize(m_o_GraphicsSystem->FetchTexture(eventData.texName));
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
	m_o_EventManagerPtr->EmitEvent<Events::EV_ENTITY_POOL_CHANGED>(Events::EV_ENTITY_POOL_CHANGED());
}

ProjectileSystem::~ProjectileSystem()
{
	m_o_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
};
