#include "System.h"
#include <iostream>


void BaseSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	std::cout << "Error 2001 : Updating a non overriden system" << std::endl;
}


short BaseSystem::GetID()
{
	return m_s_SystemID;
}
const char * BaseSystem::GetName()
{
	return m_c_SystemName;
}
void BaseSystem::SetID(short id)
{
	m_s_SystemID = id;
}
void BaseSystem::SetName(const char* name)
{
	m_c_SystemName = name;
}

BaseSystem::BaseSystem(EntityManager* entityManager)
{
	m_po_EntityManager = entityManager;
	m_po_ComponentManager = entityManager->GetComponentManager();
}

