
#include <vector>

#include "EntityManager.h"

EntityManager::EntityManager()
{
	for (int i = 0; i < Entities::EndE; i++)
		entitypool.push_back(NULL);
}

void EntityManager::addEntity(BaseEntity* eptr, Entities ent)
{
	if (eptr)
	{
		BaseEntity* vptr = entitypool[ent];

		if (vptr)
		{
			while (vptr->next)
				vptr = vptr->next;

			vptr->next = eptr;
			eptr->prev = vptr;
		}
		else
			entitypool[ent] = eptr;

		attachAllComponents(eptr, ent);
	}
}

void EntityManager::attachAllComponents(BaseEntity* eptr, Entities ent)
{
	if (eptr)
	{
		Components* cptr = NULL;
		switch (ent)
		{
		case Entities::SampleE:
			cptr = ((sampleEntity*)eptr)->basicComponents;
			break;
		}

		if (cptr)
		{
			while (*cptr != Components::EndC)
			{
				comMan->newComponent(eptr, *cptr);
				cptr++;
			}
		}
	}
}

BaseEntity* EntityManager::newEntity(Entities ent)
{
	BaseEntity* eptr = NULL;
	switch (ent)
	{
		case Entities::BaseE:
			eptr = new BaseEntity;
			break;

		case Entities::SampleE:
			eptr = (BaseEntity*)new sampleEntity;
			break;
	}

	if (eptr)
	{
		eptr->entityID = ent;
		addEntity(eptr, ent);
	}

	return eptr;
}

void EntityManager::deleteEntity(BaseComponent* com)
{
	if(com)
		deleteEntity(com->ownerEntity);
}

void EntityManager::deleteEntity(BaseEntity* ent)
{
	if (ent)
	{
		BaseEntity *pptr = ent->prev, *nptr = ent->next;

		while (ent->coe.size() > 0)
			comMan->deleteComponent(ent->coe[0]);

		if (pptr && nptr)
		{
			pptr->next = nptr;
			nptr->prev = pptr;
		}
		else if (nptr && !pptr)
		{
			entitypool[ent->entityID] = nptr;
			nptr->prev = NULL;
		}
		else if (pptr && !nptr)
			pptr->next = NULL;
		else if (!pptr && !nptr)
			entitypool[ent->entityID] = NULL;

		delete ent;
	}
}
