#include "CameraComponent.h"


CameraComponent::CameraComponent(AEVec2* screenSizePtr)
{
	m_px_ScreenSize = screenSizePtr;
	m_x_CurrentViewDistance.x = m_px_ScreenSize->x;
	m_x_CurrentViewDistance.y = m_px_ScreenSize->y;
	m_x_CameraAttributes = CameraAttributes();

}
CameraComponent::~CameraComponent()
{

}

void CameraComponent::AddToTrack(DrawComponent* pDrawObject)
{

}
