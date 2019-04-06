/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "System.h"

void BaseSystem::Initialize()
{
}

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

bool BaseSystem::operator<(const BaseSystem& rhs) const
{
	return m_s_SystemID < rhs.m_s_SystemID;
}

void BaseSystem::SetID(short id)
{
	m_s_SystemID = id;
}
void BaseSystem::SetName(const char* name)
{
	m_c_SystemName = name;
}

//BaseSystem::BaseSystem(EntityManager* entityManager)
//{
//	m_po_EntityManager = entityManager;
//	m_po_ComponentManager = entityManager->GetComponentManager();
//}
//
