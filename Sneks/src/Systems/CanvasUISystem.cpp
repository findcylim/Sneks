#include "CanvasUISystem.h"



CanvasUISystem::CanvasUISystem(EntityManager* entityManagerPtr) :
	BaseSystem(entityManagerPtr)
{


}


CanvasUISystem::~CanvasUISystem()
{


}

void CanvasUISystem::Update(float dt) 
{
	(void)dt;

}


void CanvasUISystem::Initialize()
{

}

void CanvasUISystem::AddElement(HTVector2 initPosition,CanvasElementEnum num, const char * name)
{
	switch (num)
	{
	case kCanvasTextLabel:
		CanvasTextLabelEntity* newElement = m_po_EntityManager->NewEntity<CanvasTextLabelEntity>(kEntityCanvasTextLabel, name);
		break;
	case kCanvasBasicSprite:
		CanvasBasicSpriteEntity* newElement = m_po_EntityManager->NewEntity<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, name);
		break;
	case kCanvasButton:
		CanvasButtonEntity* newElement = m_po_EntityManager->NewEntity<CanvasButtonEntity>(kEntityCanvasButton, name);
		break;
	}
}