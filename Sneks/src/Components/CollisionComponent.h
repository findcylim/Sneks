#pragma once
#include "../ECS/EntityManager.h"
#include <vector>
#include "DrawComponent.h"

class CollisionComponent : public BaseComponent
{
public:
	std::vector<unsigned int> m_i_CollisionGroupVec;
	DrawComponent*		  m_po_DrawComponent;

};
