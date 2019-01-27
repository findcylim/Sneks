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

	float lowestDistanceFromScreenEdgeX = -m_x_CurrentViewDistance.x;
	float lowestDistanceFromScreenEdgeY = -m_x_CurrentViewDistance.y;

	float maxZoomOut = -0.5f;
	for (auto i_Object : m_v_ObjectsToTrack) {
		float distFromScreenEdgeX = fabsf(i_Object->GetPosition().x) - m_x_CurrentViewDistance.x / 2;
		float distFromScreenEdgeY = fabsf(i_Object->GetPosition().y) - m_x_CurrentViewDistance.y / 2;

		
		//CAMERA ZOOM CHECKS FOR ZOOM OUT ///////////////////////////////////////////////////////////////////////

		if (distFromScreenEdgeX > -m_f_DistanceOutTolerance.x / 2 * m_x_CurrentViewDistance.x)
		{
			m_f_ZoomOutVelocity -= 0.02f;
			maxZoomOut = -1.0f;

		}else if (distFromScreenEdgeY > -m_f_DistanceOutTolerance.x / 2 * m_x_CurrentViewDistance.x)
		{
			m_f_ZoomOutVelocity -= 0.02f;
			maxZoomOut = -1.0f;
		}
		else if (distFromScreenEdgeX  > -m_f_DistanceOutTolerance.x * m_x_CurrentViewDistance.x)
		{
			m_f_ZoomOutVelocity -= 0.01f;
			maxZoomOut =-0.3f;
			//DrawObject::m_f_GlobalScale -= 0.5f * dt;// (distFromScreenEdgeX + m_f_DistanceOutTolerance.x) / m_px_ScreenSize->x * dt * 50;
		}
		else if (distFromScreenEdgeY > -m_f_DistanceOutTolerance.y * m_x_CurrentViewDistance.y)
		{
			m_f_ZoomOutVelocity -= 0.01f;
			maxZoomOut =-0.3f;
			// (distFromScreenEdgeY + m_f_DistanceOutTolerance.y) / m_px_ScreenSize->y * dt * 50;
		}

		//if i_object is within the screen
		//if (distFromScreenEdgeX < 0 && distFromScreenEdgeY < 0) {
			if (distFromScreenEdgeX > lowestDistanceFromScreenEdgeX)
				lowestDistanceFromScreenEdgeX = distFromScreenEdgeX;

			if (distFromScreenEdgeY > lowestDistanceFromScreenEdgeY)
				lowestDistanceFromScreenEdgeY = distFromScreenEdgeY;
		//}
	}

	if (DrawObject::m_f_GlobalScale < 1.0f && m_f_ZoomOutVelocity >= 0)
	{

		if (-lowestDistanceFromScreenEdgeX >= m_f_DistanceInTolerance.x * m_x_CurrentViewDistance.x &&
			-lowestDistanceFromScreenEdgeY >= m_f_DistanceInTolerance.y * m_x_CurrentViewDistance.y) {
			m_f_ZoomOutVelocity += 0.05f;

			//DrawObject::m_f_GlobalScale += 0.0008f;
		}
		//else if (-lowestDistanceFromScreenEdgeX >= 100 / DrawObject::m_f_GlobalScale &&
		//	-lowestDistanceFromScreenEdgeY >= 100 / DrawObject::m_f_GlobalScale)
		//{
		//	m_f_ZoomOutVelocity += 0.05f;

		//	//DrawObject::m_f_GlobalScale += 0.0005f;
		//}
	}


	if (m_f_ZoomOutVelocity > 0.25f)
		m_f_ZoomOutVelocity = 0.25f;
	else if (m_f_ZoomOutVelocity < maxZoomOut)
		m_f_ZoomOutVelocity = maxZoomOut;
	else if (fabsf(m_f_ZoomOutVelocity) < 0.01f)
		m_f_ZoomOutVelocity = 0;
	float speed = 1;

	if (GetAsyncKeyState(AEVK_V))
		DrawObject::m_f_GlobalScale = 1;
	//if (GetAsyncKeyState(AEVK_C))
	//	speed--;

	if (GetAsyncKeyState(AEVK_Z))
	{
		DrawObject::m_f_GlobalScale*= 0.999f;
		//DrawObject::m_f_GlobalScale += speed * 0.1f * dt;
	}
	else if (GetAsyncKeyState(AEVK_X))
	{
		//DrawObject::m_f_GlobalScale*= 0.998f;
		DrawObject::m_f_GlobalScale -= 0.1f * dt;
		//DrawObject::m_f_GlobalScale -= speed * 0.1f * dt;
		
	}
	else
		;// DrawObject::m_f_GlobalScale += m_f_ZoomOutVelocity * dt;
	m_f_ZoomOutVelocity *= 0.9f;



}
