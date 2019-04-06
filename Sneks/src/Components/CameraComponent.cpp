/* Start Header****************************************************************/
/*!
\file CameraComponent.cpp
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "CameraComponent.h"
#include "../Utility/AlphaEngineHelper.h"

CameraComponent::CameraComponent()
{
	AlphaEngineHelper::GetScreenSize(&m_px_ScreenSize.x, &m_px_ScreenSize.y);

	m_x_CurrentViewDistance.x = m_px_ScreenSize.x;
	m_x_CurrentViewDistance.y = m_px_ScreenSize.y;
	m_x_CameraAttributes = CameraAttributes();
	m_f_VirtualOffset.x = 0;
	m_f_VirtualOffset.y = 0;
	m_f_VirtualScale = 1.0f;
	m_f_VirtualRotation = 0;
	m_f_ZoomVelocity = 0;
	m_b_TrackObjects = false;
}
CameraComponent::~CameraComponent()
= default;

void CameraComponent::AddToTrack(TransformComponent* pDrawObject)
{
	m_b_TrackObjects = true;
	m_v_EntitiesToTrack.push_back(pDrawObject);
}

HTVector2 CameraComponent::GetCameraPos(bool includeShake) const
{
	if (includeShake)
		return m_f_VirtualOffset + m_f_VirtualShakeOffset;
	else
		return m_f_VirtualOffset;
}

float CameraComponent::GetScale()
{
	return m_f_VirtualScale;
}
