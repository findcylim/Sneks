#pragma once
#include "../../ECS/System.h"
#include "../../Utility/GameStateManager.h"
#include "../GraphicsSystem.h"
class CreditsScreenSystem :
	public BaseSystem,
	public EventListener<Events::EV_MOUSE_ONCLICK>
{
	State m_e_PrevState = kStateExit;
	float m_f_Timer = 3.0f;
	int m_i_PortraitValue = 0;
	GraphicsSystem* m_po_GraphicsSystem = nullptr;
	bool m_b_ClickHold = true;
	void UpdateText(CanvasTextLabelEntity* textEntity, const char * nameText, HTVector2 ScreenOffset);
public:
	void SetNextState(State nextState);
	void Initialize();
	void Receive(const Events::EV_MOUSE_ONCLICK& eventData) override;
	CreditsScreenSystem(EntityManager* entityManager, EventManager* eventManager,GraphicsSystem* graphicsSystem);
	~CreditsScreenSystem();
	void Update(float dt);
	void OnEnable() override;
	void OnDisable() override;
};

