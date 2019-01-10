
#include <vector>

#include "EntityManager.h"
#include "ComponentManager.h"

std::vector<BaseEntity*> entitypool;

void emInit()
{
	for (int i = 0; i < Entities::EndE; i++)
		entitypool.push_back(NULL);
}

void attachAllComponents(BaseEntity* eptr, Entities ent)
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
				newComponent(eptr, *cptr);
				cptr++;
			}
		}
	}
}

void addEntity(BaseEntity* eptr, Entities ent)
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

BaseEntity* newEntity(Entities ent)
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

void deleteEntity(BaseComponent* com)
{
	if(com)
		deleteEntity(com->ownerEntity);
}

void deleteEntity(BaseEntity* ent)
{
	if (ent)
	{
		BaseEntity *pptr = ent->prev, *nptr = ent->next;

		while (ent->coe.size() > 0)
			deleteComponent(ent->coe[0]);

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
