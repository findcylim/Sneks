#include "Camera.h"
#include "../Utility/AlphaEngineHelper.h"
#include <iostream>


float Camera::GetVirtualScale() const
{
	return m_f_VirtualScale;
}

float Camera::GetVirtualOffsetX() const
{
	return m_f_VirtualOffsetX;
}

float Camera::GetVirtualOffsetY() const
{
	return m_f_VirtualOffsetY;
}


Camera::Camera(AEVec2* screenSizePtr)
{
	m_px_ScreenSize = screenSizePtr;
	m_x_CurrentViewDistance.x = m_px_ScreenSize->x;
	m_x_CurrentViewDistance.y = m_px_ScreenSize->y;
	m_x_CameraAttributes = CameraAttributes();

}


Camera::~Camera()
{
}

void Camera::AddToTrack(DrawObject* pDrawObject)
{
	m_v_ObjectsToTrack.push_back(pDrawObject);
}

//void Camera::Update(float dt)
//{
//	m_x_CurrentViewDistance.x = m_px_ScreenSize->x / DrawObject::m_f_GlobalScale;
//	m_x_CurrentViewDistance.y = m_px_ScreenSize->y / DrawObject::m_f_GlobalScale;
//
//	float lowestDistanceFromScreenEdgeX = -m_x_CurrentViewDistance.x;
//	float lowestDistanceFromScreenEdgeY = -m_x_CurrentViewDistance.y;
//
//	//ZOOM OUT CHECKS
//	for (auto i_Object : m_v_ObjectsToTrack) 
//	{
//		float distFromScreenEdgeX = fabsf(i_Object->GetPosition().x )
//		- m_x_CurrentViewDistance.x / 2;
//		float distFromScreenEdgeY = fabsf(i_Object->GetPosition().y )
//		- m_x_CurrentViewDistance.y / 2;
//
//		if ((distFromScreenEdgeX > -m_f_DistanceOutTolerance.x / 2 * m_x_CurrentViewDistance.x))
//		{
//			m_i_CurrentStage = -1;
//			m_f_ZoomVelocity -= m_x_CameraAttributes.zoomOutBaseSpeed;
//		}
//		if (distFromScreenEdgeY > -m_f_DistanceOutTolerance.y / 2 * m_x_CurrentViewDistance.y)
//		{
//			m_i_CurrentStage = -1;
//			m_f_ZoomVelocity -= m_x_CameraAttributes.zoomOutBaseSpeed * 1.3f;
//		}
//		if (distFromScreenEdgeX > -m_f_DistanceOutTolerance.x * m_x_CurrentViewDistance.x)
//		{
//			m_i_CurrentStage = -2;
//			m_f_ZoomVelocity -= m_x_CameraAttributes.zoomOutBaseSpeed;
//		}
//		if (distFromScreenEdgeY > -m_f_DistanceOutTolerance.y * m_x_CurrentViewDistance.y)
//		{
//			m_i_CurrentStage = -2;
//			m_f_ZoomVelocity -= m_x_CameraAttributes.zoomOutBaseSpeed * 1.3f;
//		}
//		
//	
//
//		//Record the object nearest to the edge
//		if (distFromScreenEdgeX > lowestDistanceFromScreenEdgeX &&
//			distFromScreenEdgeY > lowestDistanceFromScreenEdgeY) 
//		{
//			lowestDistanceFromScreenEdgeX = distFromScreenEdgeX;
//			lowestDistanceFromScreenEdgeY = distFromScreenEdgeY;
//		}
//	}
//
//
//	//ZOOM IN CHECKS
//	if (DrawObject::m_f_GlobalScale < 1.0f && m_f_ZoomVelocity >= 0)
//	{
//		if (-lowestDistanceFromScreenEdgeX >= m_f_DistanceInTolerance.x * m_x_CurrentViewDistance.x &&
//			-lowestDistanceFromScreenEdgeY >= m_f_DistanceInTolerance.y * m_x_CurrentViewDistance.y) {
//			m_i_CurrentStage = 1;
//			m_f_ZoomVelocity += m_x_CameraAttributes.zoomInBaseSpeed;
//		}
//
//	}
//	/*
//	if (m_i_CurrentStage == -1)
//		m_f_ZoomVelocity -= m_x_CameraAttributes.zoomOutBaseSpeed;
//		//m_x_CameraAttributes.zoomOutMaxSpeed = m_x_CameraAttributes.zoomOutBaseSpeed * 30 * m_x_CameraAttributes.perStageMultiplier;
//	else if (m_i_CurrentStage == -2)
//		m_f_ZoomVelocity -= m_x_CameraAttributes.zoomOutBaseSpeed;
//	
//		//m_x_CameraAttributes.zoomOutMaxSpeed = m_x_CameraAttributes.zoomOutBaseSpeed * 30;
//	case 1://m_f_ZoomVelocity += m_x_CameraAttributes.zoomInBaseSpeed;
//		break;
//	case 0:
//		break;
//	default: m_f_ZoomVelocity = 0;
//	}*/
//
//	if (m_f_ZoomVelocity > m_x_CameraAttributes.zoomInMaxSpeed)
//		m_f_ZoomVelocity = m_x_CameraAttributes.zoomInMaxSpeed;
//	else if (m_f_ZoomVelocity < -m_x_CameraAttributes.zoomOutMaxSpeed)
//		m_f_ZoomVelocity = -m_x_CameraAttributes.zoomOutMaxSpeed;
//	else if (fabsf(m_f_ZoomVelocity) < 0.01f) {
//		m_f_ZoomVelocity = 0;
//		m_i_CurrentStage = 0;
//	}
//
//
//	if (AEInputCheckReleased(AEVK_V)) {
//		m_b_TrackObjects = !m_b_TrackObjects;
//	}
//
//	if (m_b_TrackObjects) {
//		HTVector2 averagePosition ={
//	(m_v_ObjectsToTrack.front()->GetPosition().x +	m_v_ObjectsToTrack.back()->GetPosition().x)
//		/ 2,
//	(m_v_ObjectsToTrack.front()->GetPosition().y +	m_v_ObjectsToTrack.back()->GetPosition().y)
//		/ 2};	
//		DrawObject::m_f_GlobalCameraOffsetX = -averagePosition.x;
//		DrawObject::m_f_GlobalCameraOffsetY = -averagePosition.y;
//	}
//
//	DrawObject::m_f_GlobalScale *= 1 + m_f_ZoomVelocity * 1.4f * dt;
//
//	m_f_ZoomVelocity *= m_x_CameraAttributes.speedDecay;
//}
