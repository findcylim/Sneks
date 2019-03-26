/* Start Header ***************************************************************/
/*!
\file CameraShake.h
\author Lim Chu Yan, chuyan.lim, 440002918
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 18/03/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 5

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef CAMERA_SHAKE_H
#define CAMERA_SHAKE_H
#pragma once
#include <AEVec2.h>
#include "../Components/CameraComponent.h"


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
	void Update(CameraComponent* camComp, float dt);
	~CameraShake();
	float AddShake(float magnitude); //Adds to the shake magnitude
	void SetShake(float magnitude); //Adds to the shake magnitude

};

#endif //CAMERA_SHAKE_H