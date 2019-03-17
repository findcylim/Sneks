#include "CameraComponent.h"
#include "../Utility/AlphaEngineHelper.h"

CameraComponent::CameraComponent()
{
	AlphaEngineHelper::GetScreenSize(&m_px_ScreenSize.x, &m_px_ScreenSize.y);

	m_x_CurrentViewDistance.x = m_px_ScreenSize.x;
	m_x_CurrentViewDistance.y = m_px_ScreenSize.y;
	m_x_CameraAttributes = CameraAttributes();
	m_f_VirtualPosition.x = 0;
	m_f_VirtualPosition.y = 0;
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

float CameraComponent::GetScale()
{
	return m_f_VirtualScale;
}
