#pragma once
#include "../ECS/System.h"
#include "GraphicsSystem.h"
class CanvasUISystem :
	public BaseSystem,
	public EventListener<Events::EV_NEW_UI_ELEMENT>,
	public EventListener<Events::EV_PLAYER_COLLISION>
{
	GraphicsSystem* m_po_GraphicsManager;

public:
	void Update(float dt) override;
	CanvasUISystem(EntityManager* entityManagerPtr,GraphicsSystem* graphicsManager, EventManager* eventManager);
	~CanvasUISystem();
	void Initialize();
	void AddElement(CanvasComponent* canvasComponent, HTVector2 initPosition, CanvasElementEnum num, const char * name,
		const char * uiElementSprite, const char* uiText = "", const char * uiHoverSprite = "", const char * uiClickSprite = "", 
		void(*ButtonFunction)() = nullptr);
	void ClearUI(CanvasComponent* canvas);
	void Receive(const Events::EV_NEW_UI_ELEMENT& eventData) override;
	void Receive(const Events::EV_PLAYER_COLLISION& eventData) override;
};
