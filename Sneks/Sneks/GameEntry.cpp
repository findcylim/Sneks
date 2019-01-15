#include <iostream>
#include <cstdio>
#include <vector>

#include"ComponentManager.h"
#include "EntityManager.h"

void printC(Component com)
{
	if (com)
		std::cout << "Sample" << std::endl;
	else
		std::cout << "Base" << std::endl;
}

void printE(Entity ent)
{
	if (ent)
		std::cout << "Sample" << std::endl;
	else
		std::cout << "Base" << std::endl;
}

int main() {
	//game goes here

	EntityManager* enMan = new EntityManager;

	BaseEntity* eptr1 = enMan->NewEntity(Entity::kEntityBase, "eptr1");
	BaseEntity* eptr2 = enMan->NewEntity(Entity::kEntitySample, "eptr2");
	BaseEntity* eptr3 = enMan->NewEntity(Entity::kEntityEnd, "eptr3");
	BaseEntity* eptr4 = enMan->NewEntity(Entity::kEntitySample, "eptr2");

	if (eptr1)
	{
		std::cout << "Entity 1: ";
		printE(eptr1->m_x_EntityID);

		enMan->m_po_ComponentManagerInstance->NewComponent(eptr1, Component::kComponentSample);

		if (eptr1->m_v_AttachedComponentsList.size())
		{
			std::cout << "Entity 1's components: ";
			printC(eptr1->m_v_AttachedComponentsList[0]->m_x_ComponentID);
		}
		else
			std::cout << "Entity 1 does not have any components." << std::endl;
	}
	else
		std::cout << "Entity 1 does not exist." << std::endl;

	std::cout << std::endl;

	if (eptr2)
	{
		std::cout << "Entity 2: ";
		printE(eptr2->m_x_EntityID);

		enMan->m_po_ComponentManagerInstance->DeleteComponent(eptr2->m_v_AttachedComponentsList[0]);

		if (eptr2->m_v_AttachedComponentsList.size())
		{
			std::cout << "Entity 2's components: ";
			printC(eptr2->m_v_AttachedComponentsList[0]->m_x_ComponentID);
		}
		else
			std::cout << "Entity 2 does not have any components." << std::endl;
	}
	else
		std::cout << "Entity 2 does not exist." << std::endl;

	std::cout << std::endl;

	if (eptr3)
	{
		std::cout << "Entity 3: ";
		printE(eptr3->m_x_EntityID);

		if (eptr3->m_v_AttachedComponentsList.size())
		{
			std::cout << "Entity 3's components: ";
			printC(eptr3->m_v_AttachedComponentsList[0]->m_x_ComponentID);
		}
		else
			std::cout << "Entity 3 does not have any components." << std::endl;
	}
	else
		std::cout << "Entity 3 does not exist." << std::endl;

	std::cout << std::endl;

	if (eptr4)
	{
		std::cout << "Entity 4: ";
		std::cout << eptr4->m_pc_EntityName << std::endl;

		for (int i = 0; i < Component::kComponentEnd; i++)
		{
			std::cout << "Entity 4 ";
			if (enMan->m_po_ComponentManagerInstance->GetSpecificComponentInstance(eptr1, (Component)i))
				std::cout << "has ";
			else
				std::cout << "does not have ";

			printC((Component)i);
		}

	}
	else
		std::cout << "Entity 4 does not exist." << std::endl;

	std::cout << std::endl;

	enMan->DeleteEntity(eptr1);
	enMan->DeleteEntity(eptr2);
	enMan->DeleteEntity(eptr3);
	enMan->DeleteEntity(eptr4);

	getchar();

	return 0;

}