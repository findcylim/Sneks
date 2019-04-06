/* Start Header ***************************************************************/
/*!
\file <name>.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: XX/XX/2019
\date Modified: XX/XX/2019
\brief This file contains the template to copy paste when creating new systems.

\par Contribution (hours): CY - 0.5

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/
#include "<name>System.h"
#include "../Components/<name>Component.h"


void <name>System::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	CollisionGroupName collGroup1 = eventData.object1->m_i_CollisionGroupVec[0];
	CollisionGroupName collGroup2 = eventData.object2->m_i_CollisionGroupVec[0];
	CollisionGroupPairing colPairing ={ collGroup1, collGroup2 };

	if(collGroup1 == kCollGroupSnek1Head &&
		collGroup2 == kCollGroupSnek2Body)
	{ 
	  //Do something
	}
}

<name>System::<name>System()
{
}

<name>System::~<name>System()
{
}

void <name>System::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this, this);
}

void <name>System::Update(float dt)
{
	m_po_ComponentManager->Each<<name>Component>([&](<name>Component* comp){
	  //Do something
	},kComponent<name>);
}