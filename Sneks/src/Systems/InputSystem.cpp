/* Start Header****************************************************************/
/*!
\file InputSystem.cpp
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains a system handling all input events

\par Contribution : CY     - 49.18% (Commented code on Callback based InputSystem)
\par Contribution : Adam   - 50.82% (Update Function)

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "InputSystem.h"
#include "../Utility/AlphaEngineHelper.h"

/**************************************************

	The benefit of creating an input manager is
	reduce the dependencies of individual files
	which require Windows.h

	Making the System slightly more efficient,
	especially when we have more systems which
	take in more player input. (e.g main menu,
	choose your character screen, game screen,
	pause screen etc...)

***************************************************/
//KeyState m_x_CurrentKeyStates[] ={
//	{ HTKEY_W,        true,    false,    100,    0 },
//	{ HTKEY_A,        true,    false,    100,    0 },
//	{ HTKEY_S,        true,    false,    100,    0 },
//	{ HTKEY_D,        true,    false,    100,    0 },
//	{ HTKEY_LEFT,     true,    false,    100,    0 },
//	{ HTKEY_RIGHT,    true,    false,    100,    0 },
//	{ HTKEY_DOWN,     true,    false,    100,    0 },
//	{ HTKEY_UP,       true,    false,    100,    0 },
//	{ HTKEY_SPACE,    true,    false,    100,    0 },
//	{ HTKEY_ENTER,    true,    false,    100,    0 },
//	{ HTKEY_ESC,      true,    false,    100,    0 },
//	{ HTKEY_E,        true,    false,    100,    0 },
//	{ HTKEY_P,        true,    false,    100,    0 },
//	{ HTKEY_C,        true,    false,    100,    0 },
//	{ HTKEY_B,        true,    false,    100,    0 },
//	{ HTKEY_NUM_1,    true,    false,    100,    0 },
//	{ HTKEY_NUM_2,    true,    false,    100,    0 },
//	{ HTKEY_NUM_3,    true,    false,    100,    0 },
//	{ HTKEY_NUM_4,    true,    false,    100,    0 },
//	{ HTKEY_NUM_5,    true,    false,    100,    0 }
//};

//constexpr size_t kKeyCount = sizeof(m_x_CurrentKeyStates) / sizeof(KeyState);

/*
	Constructor
*/
InputSystem::InputSystem()
{
	AlphaEngineHelper::GetScreenSize(&m_o_ScreenSize.x, &m_o_ScreenSize.y);
	// Sets the screen size
	m_o_ScreenSize.x *= 0.5f;
	m_o_ScreenSize.y *= 0.5f;
}

InputSystem::~InputSystem() = default;

/*
	For future use
*/
void InputSystem::SetKeyBinds()
{
	/*
	for (auto num : KeyBindsId)
	{

	}*/
}

/*
	Update function

	Currently, updates the mouse position and calculates the coordinate system conversion
*/
void InputSystem::Update(float dt)
{
	MouseEntity* mouseEntity = m_po_EntityManager->GetFirstEntityInstance<MouseEntity>(kEntityMouse);
	POINT mouse;
	
	// Get mouse screen position
	GetCursorPos(&mouse);
	TransformComponent* t_Comp = mouseEntity->GetComponent<TransformComponent>();
	CameraComponent * c_Comp = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);

	//	According to the camera scale
	//	Will update the position
	float scale = 1.0f / c_Comp->GetScale();
	t_Comp->m_f_ScaleMultiplier = (scale);
	float overScale = (1 / scale);
	float mouseOffset = 5 * overScale;
	t_Comp->m_x_Position.x = (-c_Comp->m_f_VirtualOffset.x*overScale + (mouse.x- m_o_ScreenSize.x)-mouseOffset);
	t_Comp->m_x_Position.y = (-c_Comp->m_f_VirtualOffset.y*overScale + (m_o_ScreenSize.y-mouse.y )+mouseOffset*4);

	//	 Offset from scale
	if (t_Comp->m_x_Position.x > 0)
		t_Comp->m_x_Position.x -= (t_Comp->m_x_Position.x*(1 - scale));
	else if(t_Comp->m_x_Position.x < 0)
		t_Comp->m_x_Position.x -= (t_Comp->m_x_Position.x*(1 - scale));

	if (t_Comp->m_x_Position.y > 0)
		t_Comp->m_x_Position.y -= (t_Comp->m_x_Position.y*(1 - scale));
	else if (t_Comp->m_x_Position.y < 0)
		t_Comp->m_x_Position.y -= (t_Comp->m_x_Position.y*(1 - scale));

	
	if (AEInputCheckReleased(VK_LBUTTON))
	{
		if (m_b_IsMousePressed == false)
		{
			m_b_IsMousePressed = true;
			m_po_EventManagerPtr->EmitEvent<Events::EV_MOUSE_ONCLICK>(Events::EV_MOUSE_ONCLICK{});
		}
	}
	else
	{
		m_b_IsMousePressed = false;
	}

	//CanvasButtonEntity* base = m_po_EntityManager->GetFirstEntityInstance<CanvasButtonEntity>(kEntityCanvasButton);
	//printf("X: %d Y: %d Game X: %f Game Y: %f %f %f \n", mouse.x, mouse.y,t_Comp->m_x_Position.x, t_Comp->m_x_Position.y,base->GetComponent<TransformComponent>()->m_x_Position.x, base->GetComponent<TransformComponent>()->m_x_Position.y);
	UNREFERENCED_PARAMETER(dt);

	/*
	for (i = 0; i < kKeyCount; i++)
	{
		KeyState *current_key = &m_x_CurrentKeyStates[i];
		//decrement m_i_KeyId cd timers if they are cooling down
		if (current_key->m_b_CoolDownEnabled && current_key->m_b_KeyCoolingDown)
		{
			//subtract by deltatime in ms
			if (current_key->m_f_CurrentCoolDownTimer >= 0.0f)
			{
				current_key->m_f_CurrentCoolDownTimer -= kKeyCount;
			}
			else
			{
				//finished cooling down
				current_key->m_b_KeyCoolingDown = 0;
				current_key->m_f_CurrentCoolDownTimer = 0;
			}
		}

		// if keydown
		if (GetAsyncKeyState(current_key->m_i_KeyId))
		{
			// and cooldown enabled
			if (current_key->m_b_CoolDownEnabled)
			{
				if (current_key->m_b_KeyCoolingDown)
				{
					// if it's cooling down dont fire off
					continue;
				}
				else
				{
					// fire off but set it to cooling down
					current_key->m_f_CurrentCoolDownTimer = current_key->m_f_CoolDown;
					current_key->m_b_KeyCoolingDown = 1;
				}
			}

			//PostEvent(GEV_KEYPRESS, (void*)&(current_key->m_i_KeyId));


		}
	}*/

	//State currentState = m_o_GameStateManager->ReturnCurrentState();

	////Player 1 Controls
	////'A' Key (Turn Left)
	//if (GetAsyncKeyState(65) < 0 || GetAsyncKeyState(97) < 0)
	//{
	//	switch (currentState)
	//	{
	//		case kStateGame:
	//			//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P1 TURN LEFT");
	//			m_o_EventManager->EmitEvent(Ev_PLAYER1GAME_LEFTKEY);
	//			break;
	//	}
	//}

	////'D' Key (Turn Right)
	//if (GetAsyncKeyState(68) < 0 || GetAsyncKeyState(100) < 0)
	//{
	//	switch (currentState)
	//	{
	//		case kStateGame:
	//			//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P1 TURN RIGHT");
	//			m_o_EventManager->EmitEvent(Ev_PLAYER1GAME_RIGHTKEY);
	//			break;
	//	}
	//}

	////'Left Shift' Key (Activate Power-up)
	//if (GetAsyncKeyState(VK_LSHIFT) < 0)
	//{
	//	switch (currentState)
	//	{
	//		case kStateGame:
	//			//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P1 POWERUP");
	//			m_o_EventManager->EmitEvent(Ev_PLAYER1GAME_LEFTSHIFTKEY);
	//			break;
	//	}
	//}

	////Player 2 Controls
	////'Left Arrow' Key (Turn Left)
	//if (GetAsyncKeyState(VK_LEFT) < 0 )
	//{
	//	switch (currentState)
	//	{
	//		case kStateGame:
	//			//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P2 TURN LEFT");
	//			m_o_EventManager->EmitEvent(Ev_PLAYER2GAME_LEFTKEY);
	//			break;
	//	}

	//}

	////'Right Arrow' Key (Turn Right)
	//if (GetAsyncKeyState(VK_RIGHT) < 0)
	//{
	//	switch (currentState)
	//		{
	//		case kStateGame:
	//			//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P2 TURN RIGHT");
	//			m_o_EventManager->EmitEvent(Ev_PLAYER2GAME_RIGHTKEY);
	//			break;
	//	}
	//}

	////'Right Shift' Key (Activate Power-up)
	//if (GetAsyncKeyState(VK_RSHIFT) < 0)
	//{
	//	switch (currentState)
	//	{
	//		case kStateGame:
	//			m_o_EventManager->EmitEvent(Ev_PLAYER2GAME_RIGHTSHIFTKEY);
	//			break;
	//		}
	//}
}
