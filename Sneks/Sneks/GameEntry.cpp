#include <iostream>
#include <cstdio>
#include <vector>

#include"ComponentManager.h"
#include "EntityManager.h"

void printC(Components com)
{
	if (com)
		std::cout << "Sample" << std::endl;
	else
		std::cout << "Base" << std::endl;
}

void printE(Entities ent)
{
	if (ent)
		std::cout << "Sample" << std::endl;
	else
		std::cout << "Base" << std::endl;
}

int main() {
	//game goes here

	cmInit();
	emInit();

	BaseEntity* eptr1 = newEntity(Entities::BaseE);
	BaseEntity* eptr2 = newEntity(Entities::SampleE);
	BaseEntity* eptr3 = newEntity(Entities::EndE);

	if (eptr1)
	{
		std::cout << "Entity 1: ";
		printE(eptr1->entityID);

		if (eptr1->coe.size())
		{
			std::cout << "Entity 1's components: ";
			printC(eptr1->coe[0]->componentID);
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
		printE(eptr2->entityID);

		if (eptr2->coe.size())
		{
			std::cout << "Entity 2's components: ";
			printC(eptr2->coe[0]->componentID);
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
		printE(eptr3->entityID);

		if (eptr3->coe.size())
		{
			std::cout << "Entity 3's components: ";
			printC(eptr3->coe[0]->componentID);
		}
		else
			std::cout << "Entity 3 does not have any components." << std::endl;
	}
	else
		std::cout << "Entity 3 does not exist." << std::endl;

	std::cout << std::endl;

	deleteEntity(eptr1);
	deleteEntity(eptr2);
	deleteEntity(eptr3);

	getchar();

	return 0;

}