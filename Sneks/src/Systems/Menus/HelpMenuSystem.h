#pragma once
#include "../../ECS/System.h"
#include "../../Utility/GameStateManager.h"
class HelpMenuSystem :
	public BaseSystem,
	public EventListener<Events::EV_MOUSE_ONCLICK>
{
	State m_e_PrevState = kStateExit;
	float m_f_Timer = 0.5f;
public:
	void SetNextState(State nextState);
	void Initialize();
	void Receive(const Events::EV_MOUSE_ONCLICK& eventData) override;
	HelpMenuSystem();
	~HelpMenuSystem();
	void Update(float dt);
	void OnEnable() override;
	void OnDisable() override;
};

