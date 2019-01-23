#pragma once
#include <AEVec2.h>


class CameraShake
{

private:
	int    m_i_ShakeAngleMagnitude;
	int    m_f_CurrentShakeAngle;

	float  m_i_ShakeInterval; //If dt > m_i_ShakeInterval, then update screenshake magnitude
	float  m_f_CurrentShakeMagnitude;//
	float  m_f_ShakeTimer;	  // to facilitate the interval
	AEVec2 m_x_CameraOffset;
	AEVec2 m_x_ScreenCentre;


public:
	CameraShake();
	void Update(float dt);
	~CameraShake();
	float AddShake(float magnitude); //Adds to the shake magnitude
	void SetShake(float magnitude); //Adds to the shake magnitude

};

