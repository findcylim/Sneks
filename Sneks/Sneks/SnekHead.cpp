#include "SnekHead.h"


#define MAXVELOCITY 5.0f
#define ACCEL_FAC 0.05f
#define BRAKE_FAC 0.05f
#define TURN_SPEED 0.01f
#define FRICTION 0.01f

void SnekHead::Update()
{


	/*this shouldnt be here, only for testing*/
	if (GetAsyncKeyState(VK_UP)) {
		SetVelocity(GetVelocity() - ACCEL_FAC);
	}
	if (GetAsyncKeyState(VK_DOWN) && (GetVelocity() < 0 )) {
		SetVelocity(GetVelocity() + BRAKE_FAC);
	}
	if (GetAsyncKeyState(VK_LEFT) && (GetVelocity() <= -0.2f)) {
		SetRotation(GetRotation() + TURN_SPEED);
	}
	if (GetAsyncKeyState(VK_RIGHT) && (GetVelocity() <= -0.2f)) {
		SetRotation(GetRotation() - TURN_SPEED);
	}
	/*end removal*/

	//limit max velocity
	if (m_fVelocity >= MAXVELOCITY)
		m_fVelocity = MAXVELOCITY;
	else if (m_fVelocity <= -MAXVELOCITY)
		m_fVelocity = -MAXVELOCITY;
	
	//if out of screen, pop to the other side of screen X
	if (m_fPositionX > AEGfxGetWinMaxX() + m_fSizeX / 2)
		m_fPositionX = AEGfxGetWinMinX() - m_fSizeX / 2;
	else if (m_fPositionX < AEGfxGetWinMinX() - m_fSizeX / 2)
		m_fPositionX = AEGfxGetWinMaxX() + m_fSizeX / 2;

	//if out of screen, pop to the other side of screen Y
	if (m_fPositionY > AEGfxGetWinMaxY() + m_fSizeY / 2)
		m_fPositionY = AEGfxGetWinMinY() - m_fSizeY / 2;
	else if (m_fPositionY < AEGfxGetWinMinY() - m_fSizeY / 2)
		m_fPositionY = AEGfxGetWinMaxY() + m_fSizeY / 2;


	/*clamp low velocity to 0 so its not jittery*/
	if (m_fVelocity >= -0.05 && m_fVelocity <= 0.05)
		m_fVelocity = 0;
	else if (m_fVelocity < 0)
		m_fVelocity += FRICTION;
	else if (m_fVelocity > 0)
		m_fVelocity -= FRICTION;

	//apply the velocity
	AEVec2 testPos;
	AEVec2FromAngle(&testPos, m_fRotation);

	m_fPositionX += testPos.x * m_fVelocity;
	m_fPositionY += testPos.y * m_fVelocity;

}
