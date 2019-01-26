#include "Camera.h"
#include <iostream>


Camera::Camera(AEVec2* screenSizePtr)
{
	m_px_ScreenSize = screenSizePtr;
	m_x_CurrentViewDistance.x = m_px_ScreenSize->x;
	m_x_CurrentViewDistance.y = m_px_ScreenSize->y;

}


Camera::~Camera()
{
}

void Camera::AddToTrack(DrawObject* pDrawObject)
{
	m_v_ObjectsToTrack.push_back(pDrawObject);
}

void Camera::Update(float dt)
{
	m_x_CurrentViewDistance.x = m_px_ScreenSize->x / DrawObject::m_f_GlobalScale;
	m_x_CurrentViewDistance.y = m_px_ScreenSize->y / DrawObject::m_f_GlobalScale;

	float highestDistanceFromScreenEdgeX = 0;
	float highestDistanceFromScreenEdgeY = 0;

	for (auto i_Object : m_v_ObjectsToTrack) {
		float distFromScreenEdgeX = fabsf(i_Object->GetPosition().x) - m_x_CurrentViewDistance.x / 2;
		float distFromScreenEdgeY = fabsf(i_Object->GetPosition().y) - m_x_CurrentViewDistance.y / 2;

		//CAMERA ZOOM CHECKS FOR ZOOM OUT ///////////////////////////////////////////////////////////////////////
		if (distFromScreenEdgeX + m_f_DistanceOutTolerance.x / DrawObject::m_f_GlobalScale > 0)
		{
			DrawObject::m_f_GlobalScale -= 0.004f;// (distFromScreenEdgeX + m_f_DistanceOutTolerance.x) / m_px_ScreenSize->x * dt * 50;
		}
		else if (distFromScreenEdgeY + m_f_DistanceOutTolerance.y / DrawObject::m_f_GlobalScale > 0)
		{
			DrawObject::m_f_GlobalScale -= 0.004f;// (distFromScreenEdgeY + m_f_DistanceOutTolerance.y) / m_px_ScreenSize->y * dt * 50;
		}

		if (-distFromScreenEdgeX > highestDistanceFromScreenEdgeX)
			highestDistanceFromScreenEdgeX = distFromScreenEdgeX;

		if (-distFromScreenEdgeY > highestDistanceFromScreenEdgeY)
			highestDistanceFromScreenEdgeY = distFromScreenEdgeY;
		
	}

	if (DrawObject::m_f_GlobalScale < 1.0f)
	{

		if (-highestDistanceFromScreenEdgeX >= m_f_DistanceInTolerance.x / DrawObject::m_f_GlobalScale &&
			 -highestDistanceFromScreenEdgeY >= m_f_DistanceInTolerance.y / DrawObject::m_f_GlobalScale) {
			DrawObject::m_f_GlobalScale += 0.0008f;
		}
		else if (-highestDistanceFromScreenEdgeX >= 100 / DrawObject::m_f_GlobalScale &&
			      -highestDistanceFromScreenEdgeY >= 100 / DrawObject::m_f_GlobalScale)
		{
			DrawObject::m_f_GlobalScale += 0.0005f;
		}
	}


}
