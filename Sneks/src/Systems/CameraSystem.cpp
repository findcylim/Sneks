#include "CameraSystem.h"
#include "../Components/CameraComponent.h"


CameraSystem::CameraSystem(EntityManager* entityManagerPtr):
BaseSystem(entityManagerPtr)
{
	
}
void CameraSystem::UpdateCamera(const float dt) const
{
	auto cameraComponent = static_cast<CameraComponent*>(m_po_ComponentManager->GetFirstComponentInstance(kComponentCamera));
	while (cameraComponent) {
		cameraComponent->m_x_CurrentViewDistance.x = cameraComponent->m_px_ScreenSize.x / cameraComponent->m_f_VirtualScale;
		cameraComponent->m_x_CurrentViewDistance.y = cameraComponent->m_px_ScreenSize.y / cameraComponent->m_f_VirtualScale;

		float lowestDistanceFromScreenEdgeX = -cameraComponent->m_x_CurrentViewDistance.x;
		float lowestDistanceFromScreenEdgeY = -cameraComponent->m_x_CurrentViewDistance.y;

		//ZOOM OUT CHECKS
		for (auto i_Object : cameraComponent->m_v_EntitiesToTrack)
		{
			float distFromScreenEdgeX = fabsf(i_Object->GetPosition().x)
				- cameraComponent->m_x_CurrentViewDistance.x / 2;
			float distFromScreenEdgeY = fabsf(i_Object->GetPosition().y)
				- cameraComponent->m_x_CurrentViewDistance.y / 2;

			if ((distFromScreenEdgeX > -cameraComponent->m_f_DistanceOutTolerance.x / 2 * cameraComponent->m_x_CurrentViewDistance.x))
			{
				cameraComponent->m_i_CurrentStage = -1;
				cameraComponent->m_f_ZoomVelocity -= cameraComponent->m_x_CameraAttributes.zoomOutBaseSpeed;
			}
			if (distFromScreenEdgeY > -cameraComponent->m_f_DistanceOutTolerance.y / 2 * cameraComponent->m_x_CurrentViewDistance.y)
			{
				cameraComponent->m_i_CurrentStage = -1;
				cameraComponent->m_f_ZoomVelocity -= cameraComponent->m_x_CameraAttributes.zoomOutBaseSpeed * 1.3f;
			}
			if (distFromScreenEdgeX > -cameraComponent->m_f_DistanceOutTolerance.x * cameraComponent->m_x_CurrentViewDistance.x)
			{
				cameraComponent->m_i_CurrentStage = -2;
				cameraComponent->m_f_ZoomVelocity -= cameraComponent->m_x_CameraAttributes.zoomOutBaseSpeed;
			}
			if (distFromScreenEdgeY > -cameraComponent->m_f_DistanceOutTolerance.y * cameraComponent->m_x_CurrentViewDistance.y)
			{
				cameraComponent->m_i_CurrentStage = -2;
				cameraComponent->m_f_ZoomVelocity -= cameraComponent->m_x_CameraAttributes.zoomOutBaseSpeed * 1.3f;
			}



			//Record the object nearest to the edge
			if (distFromScreenEdgeX > lowestDistanceFromScreenEdgeX &&
				distFromScreenEdgeY > lowestDistanceFromScreenEdgeY)
			{
				lowestDistanceFromScreenEdgeX = distFromScreenEdgeX;
				lowestDistanceFromScreenEdgeY = distFromScreenEdgeY;
			}
		}


		//ZOOM IN CHECKS
		if (cameraComponent->m_f_VirtualScale < 1.0f && cameraComponent->m_f_ZoomVelocity >= 0)
		{
			if (-lowestDistanceFromScreenEdgeX >= cameraComponent->m_f_DistanceInTolerance.x * cameraComponent->m_x_CurrentViewDistance.x &&
				-lowestDistanceFromScreenEdgeY >= cameraComponent->m_f_DistanceInTolerance.y * cameraComponent->m_x_CurrentViewDistance.y) {
				cameraComponent->m_i_CurrentStage = 1;
				cameraComponent->m_f_ZoomVelocity += cameraComponent->m_x_CameraAttributes.zoomInBaseSpeed;
			}

		}
		/*TODO:: Stage based camera zoom (if further then zoom slower)
		if (m_i_CurrentStage == -1)
			m_f_ZoomVelocity -= m_x_CameraAttributes.zoomOutBaseSpeed;
			//m_x_CameraAttributes.zoomOutMaxSpeed = m_x_CameraAttributes.zoomOutBaseSpeed * 30 * m_x_CameraAttributes.perStageMultiplier;
		else if (m_i_CurrentStage == -2)
			m_f_ZoomVelocity -= m_x_CameraAttributes.zoomOutBaseSpeed;

			//m_x_CameraAttributes.zoomOutMaxSpeed = m_x_CameraAttributes.zoomOutBaseSpeed * 30;
		case 1://m_f_ZoomVelocity += m_x_CameraAttributes.zoomInBaseSpeed;
			break;
		case 0:
			break;
		default: m_f_ZoomVelocity = 0;
		}*/

		if (cameraComponent->m_f_ZoomVelocity > cameraComponent->m_x_CameraAttributes.zoomInMaxSpeed)
			cameraComponent->m_f_ZoomVelocity = cameraComponent->m_x_CameraAttributes.zoomInMaxSpeed;
		else if (cameraComponent->m_f_ZoomVelocity < -cameraComponent->m_x_CameraAttributes.zoomOutMaxSpeed)
			cameraComponent->m_f_ZoomVelocity = -cameraComponent->m_x_CameraAttributes.zoomOutMaxSpeed;
		else if (fabsf(cameraComponent->m_f_ZoomVelocity) < 0.01f) {
			cameraComponent->m_f_ZoomVelocity = 0;
			cameraComponent->m_i_CurrentStage = 0;
		}


		if (AEInputCheckReleased(AEVK_V)) {
			cameraComponent->m_b_TrackObjects = !cameraComponent->m_b_TrackObjects;
		}

		if (cameraComponent->m_b_TrackObjects) {
			HTVector2 averagePosition ={
		(cameraComponent->m_v_EntitiesToTrack.front()->GetPosition().x + cameraComponent->m_v_EntitiesToTrack.back()->GetPosition().x)
			/ 2,
		(cameraComponent->m_v_EntitiesToTrack.front()->GetPosition().y + cameraComponent->m_v_EntitiesToTrack.back()->GetPosition().y)
			/ 2 };
			cameraComponent->m_f_VirtualOffsetX = -averagePosition.x;
			cameraComponent->m_f_VirtualOffsetY = -averagePosition.y;
		}

		cameraComponent->m_f_VirtualScale *= 1 + cameraComponent->m_f_ZoomVelocity * 1.4f * dt;

		cameraComponent->m_f_ZoomVelocity *= cameraComponent->m_x_CameraAttributes.speedDecay;

		cameraComponent = static_cast<CameraComponent*>(cameraComponent->m_po_NextComponent);
	}
}

void CameraSystem::Update(float dt)
{
	UpdateCamera(dt);
}
