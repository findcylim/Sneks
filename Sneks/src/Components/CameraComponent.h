
#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"
#include "../Components/DrawComponent.h"
#include "AEEngine.h"
#include <vector>
#include "TransformComponent.h"

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

	AEVec2				  m_x_CurrentViewDistance;
	AEVec2				  m_px_ScreenSize;
	float					  m_f_ZoomVelocity;

	//TRACKING STUFF
	bool					  m_b_TrackObjects;
	AEVec2              m_f_DistanceOutTolerance ={ 0.05f,0.042f };
	AEVec2              m_f_DistanceInTolerance  ={ 0.2f,0.2f };
	CameraAttributes    m_x_CameraAttributes;
	int					  m_i_CurrentStage;

	//ZOOM AND OFFSET ( CAMERA "X Y Z" )
	float		           m_f_VirtualOffsetX;
	float		           m_f_VirtualOffsetY;
	float		           m_f_VirtualScale;
	float					  m_f_VirtualRotation;

	void AddToTrack(TransformComponent* pDrawObject);
	void Update(float dt);

	CameraComponent();
	~CameraComponent();
};


#endif