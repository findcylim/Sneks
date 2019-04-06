/* Start Header****************************************************************/
/*!
\file HelpMenuSystem.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code for Help menu tutorial

\par Contribution : Adam - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

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

