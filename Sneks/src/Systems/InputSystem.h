/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H
#pragma once

#include "../ECS/System.h"
#include "../ECS/EventManager.h"
#include "../Utility/Logger.h"
#include "../Utility/GameStateManager.h"
#include "../ECS/EventListener.h"
#include "../Components/CameraComponent.h"

enum KeyBindsId
{
	HTKEY_W = 0x57,
	HTKEY_A = 0x41,
	HTKEY_S = 0x53,
	HTKEY_D = 0x44,
	HTKEY_LEFT = 0x25,
	HTKEY_RIGHT = 0x27,
	HTKEY_DOWN = 0x28,
	HTKEY_UP = 0x26,
	HTKEY_SPACE = 0x20,
	HTKEY_ENTER = 0x0D,
	HTKEY_ESC = 0x1B,
	HTKEY_E = 0x45,
	HTKEY_P = 0x50,
	HTKEY_C = 0x43,
	HTKEY_B = 0x42,
	HTKEY_NUM_1 = 0x31,
	HTKEY_NUM_2 = 0x32,
	HTKEY_NUM_3 = 0x33,
	HTKEY_NUM_4 = 0x34,
	HTKEY_NUM_5 = 0x35
};

enum ButtonNames
{
	kInputPlayer1TurnLeft,
	kInputPlayer1TurnRight,
	kInputPlayer1PowerUp,
	kInputPlayer2TurnLeft,
	kInputPlayer2TurnRight,
	kInputPlayer2PowerUp,
	kEscape,
};

struct KeyState
{
	const unsigned m_i_KeyId;                 //the m_i_KeyId
	const bool     m_b_CoolDownEnabled;       //If CoolDown Enabled
	const float    m_f_CoolDown;              //Value in ms
	float          m_f_CurrentCoolDownTimer;  //The timer to keep track of whether it's on CD
	//eventID?
	KeyState(unsigned keyId, float coolDown);
};

class InputSystem : public BaseSystem
{
public:
	InputSystem();
	~InputSystem();
	void Update(float dt) override;

private:
	std::vector<KeyState> m_x_CurrentKeyStates;
	HTVector2 m_o_ScreenSize;
	void SetKeyBinds();
	void ResolveKeyPress(unsigned char keyId);
};

#endif