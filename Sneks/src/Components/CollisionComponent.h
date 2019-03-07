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
	kCollGroupMouse
};

class CollisionComponent : public BaseComponent
{
public:
	std::vector<CollisionGroupName>   m_i_CollisionGroupVec;
	DrawComponent*					  m_po_DrawComponent;
	bool							  enabled = true;
	bool							  m_b_Colliding = false;
};