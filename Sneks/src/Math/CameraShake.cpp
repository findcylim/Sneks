#include "CameraShake.h"
#include <AEVec2.h>
#include <cstdlib>
#include <AEEngine.h>
#include "../Components/CameraComponent.h"


CameraShake::CameraShake()
{
	m_i_ShakeAngleMagnitude        = 180;
	m_i_ShakeInterval              = 0.05f;

	m_x_ScreenCentre               = {0,0};
	m_f_CurrentShakeMagnitude      = 0;
	m_f_CurrentShakeAngle          = rand() % 360;
	m_f_ShakeTimer                 = 0;
	m_x_CameraOffset               = { 0,0};

	//TODO :: CHANGE TO GRAPHICS.H INSTEAD OF INCLUDING AEGRAPHICS HERE
	AEGfxGetCamPosition(&m_x_ScreenCentre.x, &m_x_ScreenCentre.y);

}

void CameraShake::Update(CameraComponent* camComp, float dt)
{
	m_f_ShakeTimer += dt;
	//Updates the angle and shake magnitude when time every ShakeInterval seconds
	if (m_f_ShakeTimer > m_i_ShakeInterval && m_f_CurrentShakeMagnitude > 0) {
		m_f_ShakeTimer = 0;

		m_f_CurrentShakeMagnitude *= 0.9f; //diminish radius each frame
		m_f_CurrentShakeAngle +=(180 + (-(m_i_ShakeAngleMagnitude / 2) + rand() % m_i_ShakeAngleMagnitude)); //pick new angle 

		m_x_CameraOffset.x = static_cast<float>(sin(m_f_CurrentShakeAngle) * m_f_CurrentShakeMagnitude);
		m_x_CameraOffset.y = static_cast<float>(cos(m_f_CurrentShakeAngle) * m_f_CurrentShakeMagnitude);
		//Shakes the Camera
		//AEGfxSetCamPosition(m_x_ScreenCentre.x + m_x_CameraOffset.x, m_x_ScreenCentre.y + m_x_CameraOffset.y);
		camComp->m_f_VirtualOffset.x = m_x_CameraOffset.x;
		camComp->m_f_VirtualOffset.y = m_x_CameraOffset.y;

		//Don't micro shake the screen, also returns the screen back to centre after shaking is done
		if (m_f_CurrentShakeMagnitude < 0.2f) {
			m_f_CurrentShakeMagnitude = 0;
			camComp->m_f_VirtualOffset = {0,0};
			//AEGfxSetCamPosition(m_x_ScreenCentre.x, m_x_ScreenCentre.y);
		}
	}
}


CameraShake::~CameraShake() = default;

float CameraShake::AddShake(float magnitude)
{
	m_f_CurrentShakeMagnitude+= magnitude;
	return m_f_CurrentShakeMagnitude;
}

void CameraShake::SetShake(float magnitude)
{
	m_f_CurrentShakeMagnitude= magnitude;
}
