#pragma once
#include "../../ECS/System.h"
#include "../../Utility/GameStateManager.h"
class ConfirmationScreenSystem :
	public BaseSystem
{
	bool m_b_ClickHold = false;
public:
	static State m_e_PrevState;
	ConfirmationScreenSystem();
	void Update(float dt) override;
	virtual ~ConfirmationScreenSystem();
	void OnEnable()override;
	void OnDisable()override;
	void Initialize()override;
	void SetNextState(State nextState);
};


