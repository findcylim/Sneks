
#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"
#include "../Components/DrawComponent.h"
#include "AEEngine.h"
#include <vector>
#include "TransformComponent.h"
#include "../Math/HTVector2.h"

enum CameraState
{
	kCameraZoomIn = 0,
	kCameraZoomOut,
	kCameraIdle
}; 

struct CameraAttributes
{
	float				zoomInBaseSpeed    = 0.02f;
	float				zoomOutBaseSpeed   = 0.015f;
	float				zoomInMaxSpeed     = 0.25f;
	float				zoomOutMaxSpeed    = 1.0f;
	float				numStages          = 2;
	float				perStageMultiplier = 2.0f;
	float				speedDecay         = 0.9f;
};

class CameraComponent : public BaseComponent
{
public:
	std::vector<TransformComponent*> m_v_EntitiesToTrack;

	HTVector2			  m_x_CurrentViewDistance;
	HTVector2			  m_px_ScreenSize;
	float					  m_f_ZoomVelocity;

	//TRACKING STUFF
	bool					  m_b_TrackObjects;
	HTVector2           m_f_DistanceOutTolerance ={ 0.05f,0.15f };
	HTVector2           m_f_DistanceInTolerance  ={ 0.2f,0.2f };
	CameraAttributes    m_x_CameraAttributes;

	//ZOOM AND OFFSET ( CAMERA "X Y Z" )
	HTVector2			  m_f_TargetOffset;
	HTVector2			  m_f_PanningSpeed = {1.0f, 2.0f};
	HTVector2		     m_f_VirtualOffset;
	float		           m_f_VirtualScale;
	float					  m_f_VirtualRotation;
	HTVector2		     m_f_VirtualShakeOffset = {0,0};

	void AddToTrack(TransformComponent* pDrawObject);
	HTVector2 GetCameraPos(bool includeShake = false) const;
	CameraComponent();
	~CameraComponent();
	
	float GetScale();
};


#endif