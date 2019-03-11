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

InputSystem::InputSystem(EntityManager* entityManagerPtr, EventManager* eventManager, short id, const char * name, GameStateManager* gameStateManager, Logger* logger) :
	BaseSystem(entityManagerPtr)
{
	m_o_Logger				= logger;
	m_o_EventManager		= eventManager;
	m_o_GameStateManager	= gameStateManager;
	this->SetID(id);
	this->SetName(name);
	AlphaEngineHelper::GetScreenSize(&m_o_ScreenSize.x, &m_o_ScreenSize.y);
}

InputSystem::~InputSystem() = default;

void InputSystem::SetKeyBinds()
{
	/*
	for (auto num : KeyBindsId)
	{

	}*/
}

void InputSystem::Update(float dt)
{
	MouseEntity* mouseEntity = m_po_EntityManager->GetFirstEntityInstance<MouseEntity>(kEntityMouse);
	POINT mouse;
	
	GetCursorPos(&mouse);
	TransformComponent* t_Comp = mouseEntity->GetComponent<TransformComponent>();
	CameraComponent * c_Comp = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	
	float scale = 1.0f / c_Comp->GetScale();
	t_Comp->SetScale(scale);
	t_Comp->SetPositionX(-c_Comp->GetOffsetX() + (mouse.x  * scale) - m_o_ScreenSize.x *0.5f * scale);
	t_Comp->SetPositionY(-c_Comp->GetOffsetY() - (mouse.y  * scale) + m_o_ScreenSize.y *0.5f * scale);

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
