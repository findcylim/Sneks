/* Start Header****************************************************************/
/*!
\file CreditsScreenSystem.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains the UI and menus for the Credits Screen

\par Contribution : Adam   - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef CREDITS_SCREEN_SYSTEM_H
#define CREDITS_SCREEN_SYSTEM_H
#pragma once
#include "../../ECS/System.h"
#include "../../Utility/GameStateManager.h"
#include "../GraphicsSystem.h"

/*
	Credits Screen System
*/
class CreditsScreenSystem :
	public BaseSystem,
	public EventListener<Events::EV_MOUSE_ONCLICK>
{
	State m_e_PrevState = kStateExit;
	//Initial timer value
	float m_f_Timer = 3.0f;
	//First portrait to show
	int m_i_PortraitValue = 0;
	GraphicsSystem* m_po_GraphicsSystem = nullptr;
	bool m_b_ClickHold = true;
	void UpdateText(CanvasTextLabelEntity* textEntity, const char * nameText, HTVector2 ScreenOffset);
public:
	void SetNextState(State nextState);
	void Initialize();
	void Receive(const Events::EV_MOUSE_ONCLICK& eventData) override;
	CreditsScreenSystem(GraphicsSystem* graphicsSystem);
	~CreditsScreenSystem();
	void Update(float dt);
	void OnEnable() override;
	void OnDisable() override;
};

#endif