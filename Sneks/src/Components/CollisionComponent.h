/* Start Header ***************************************************************/
/*!
\file CollisionComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once
#include "../ECS/EntityManager.h"
#include <vector>
#include "DrawComponent.h"

enum CollisionGroupName
{
	kCollGroupSnek1Head = 0,
	kCollGroupSnek1Body,
	kCollGroupSnek2Head,
	kCollGroupSnek2Body,
	kCollGroupSnek3Head,
	kCollGroupSnek3Body,
	kCollGroupSnek4Head,
	kCollGroupSnek4Body,
	kCollGroupSnek5Head,
	kCollGroupSnek5Body,
	kCollGroupBuilding = 10,
	kCollGroupMoon = 11,
	kCollGroupUIButton,
	kCollGroupMouse,
	kCollGroupPowerUp
};

class CollisionComponent : public BaseComponent
{
public:
	std::vector<CollisionGroupName>   m_i_CollisionGroupVec;
	DrawComponent*					  m_po_DrawComponent;
	bool							  enabled = true;
	bool							  m_b_Colliding = false;
	int							  m_i_NumCollisionsAllowedPerFrame = 1;
	int							  m_i_CollisionsThisFrame = 0;
};