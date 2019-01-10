//make functions to check and create new, as well as delete
//please use vector to store ptr, name can be gotten from the ptr itself

#include <vector>

#include "ComponentManager.h"

std::vector<BaseComponent*> componentpool;

void cmInit()
{
	for (int i = 0; i < Components::EndC; i++)
		componentpool.push_back(NULL);
}

void addComponent(BaseComponent* cptr, Components com)
{
	if (cptr)
	{
		BaseComponent* vptr = componentpool[com];

		if (vptr)
		{
			while (vptr->next)
				vptr = vptr->next;

			vptr->next = cptr;
			cptr->prev = vptr;
		}
		else
			componentpool[com] = cptr;
	}
}

BaseComponent* newComponent(BaseEntity* ent, Components com)
{
	BaseComponent* cptr = NULL;

	if (ent)
	{
		switch (com)
		{
		case Components::BaseC:
			cptr = new BaseComponent;
			break;
		case Components::SampleC:
			cptr = (BaseComponent*)new sampleComponent;
			break;
		}

		if (cptr)
		{
			cptr->componentID = com;
			cptr->ownerEntity = ent;
			ent->coe.push_back(cptr);
		}
	}

	return cptr;
}

void deleteComponent(BaseEntity* ent, Components com)
{
	if (ent)
	{
		for (int i = 0; i < ent->coe.size(); i++)
		{
			if (ent->coe[i]->componentID == com)
				deleteComponent(ent->coe[i]);
		}
	}
}

void deleteComponent(BaseComponent* com)
{
	if (com)
	{
		BaseComponent *pptr = com->prev, *nptr = com->next;
		int i;

		if (pptr && nptr)
		{
			pptr->next = nptr;
			nptr->prev = pptr;
		}
		else if (nptr && !pptr)
		{
			componentpool[com->componentID] = nptr;
			nptr->prev = NULL;
		}
		else if (pptr && !nptr)
			pptr->next = NULL;
		else if (!pptr && !nptr)
			componentpool[com->componentID] = NULL;

		for (i = 0; com->ownerEntity->coe[i] != com; i++);
		com->ownerEntity->coe.erase(com->ownerEntity->coe.begin() + i);

		delete com;
	}
}
