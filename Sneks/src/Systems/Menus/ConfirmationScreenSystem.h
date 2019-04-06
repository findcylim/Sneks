/* Start Header****************************************************************/
/*!
\file ConfirmationScreenSystem.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Adam   - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/
#ifndef CONFIRMATION_SCREEN_SYSTEM_H
#define CONFIRMATION_SCREEN_SYSTEM_H
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


#endif