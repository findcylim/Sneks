#include "HUDSystem.h"
#include "../../Math/HTVector2.h"
#include "../../Utility/AlphaEngineHelper.h"


HUDSystem::HUDSystem(EntityManager* entityManagerPtr, EventManager* eventManager)
	:BaseSystem(entityManagerPtr)
{
	m_o_EventManagerPtr = eventManager;
	auto cameraComponent = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	cameraComponent->m_f_VirtualOffsetX = -AEGfxGetWinMaxX();
	cameraComponent->m_f_VirtualOffsetY = AEGfxGetWinMaxY();
	cameraComponent->m_f_VirtualScale = 1.0f;
}

HUDSystem::~HUDSystem()
{
	/*
	auto UI = m_po_ComponentManager->GetFirstComponentInstance<CanvasComponent>(kComponentCanvas);
	while (UI)
	{
		for (auto& element : UI->m_x_CanvasElementList)
		{
			m_po_EntityManager->AddToDeleteQueue(element);
		}
		UI->m_x_CanvasElementList.clear();
	}*/
}

void HUDSystem::Initialize(CanvasComponent* canvasComponent)
{
	CameraComponent * c_Comp = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	TransformComponent * t_Comp = m_po_ComponentManager->GetFirstComponentInstance<TransformComponent>(kComponentTransform);

	float screenX = 0, screenY = 0;
	AlphaEngineHelper::GetScreenSize(&screenX, &screenY);
	Events::EV_NEW_UI_ELEMENT HUDElement = { canvasComponent,HTVector2{ screenX / 16 , screenY / 16 } ,kCanvasBasicSprite, "Display", "HUD","","","", nullptr };
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(HUDElement);

	t_Comp->SetPosition(t_Comp->GetPosition().x + c_Comp->GetOffsetX(), t_Comp->GetPosition().y + c_Comp->GetOffsetY());
	t_Comp->SetScale(c_Comp->GetScale());
}

void HUDSystem::Update(float dt)
{
	(void)dt;
	CameraComponent * c_Comp = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	TransformComponent * t_Comp = m_po_ComponentManager->GetFirstComponentInstance<TransformComponent>(kComponentTransform);

	t_Comp->SetPosition(t_Comp->GetPosition().x + c_Comp->GetOffsetX(), t_Comp->GetPosition().y + c_Comp->GetOffsetY());
	t_Comp->SetScale(c_Comp->GetScale());
}