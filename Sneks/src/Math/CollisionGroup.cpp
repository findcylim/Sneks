#include "CollisionGroup.h"
#include "../Systems/CollisionSystem.h"

void CollisionGroup::UpdateHitBoxes()
{
	if (objects.size() == objectsHitBoxes.size())
	{
		for (int i_Objects = 0; i_Objects < objects.size(); i_Objects++)
		{
			objectsHitBoxes.at(i_Objects)->min = CollisionSystem::GetMin(objects.at(i_Objects)->m_po_DrawComponent);
			objectsHitBoxes.at(i_Objects)->max = CollisionSystem::GetMax(objects.at(i_Objects)->m_po_DrawComponent);
		}
	}
}
