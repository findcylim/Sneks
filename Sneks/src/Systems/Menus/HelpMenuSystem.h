/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 18/03/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

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

