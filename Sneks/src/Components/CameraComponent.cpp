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
	m_i_CurrentStage = 0;
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
