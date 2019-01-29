
#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"
#include "../Components/DrawComponent.h"
#include "AEEngine.h"
#include <vector>

enum CameraState
{
	kCameraZoomIn = 0,
	kCameraZoomOut,
	kCameraIdle
}; 

struct CameraAttributes
{
	float					  zoomInBaseSpeed = 0.02f;
	float					  zoomOutBaseSpeed = 0.015f;
	float					  zoomInMaxSpeed = 0.25f;
	float					  zoomOutMaxSpeed = 1.0f;
	float					  numStages = 2;
	float					  perStageMultiplier = 2.0f;
	float					  speedDecay = 0.9f;
};

class CameraComponent : public BaseComponent
{
private:
	std::vector<DrawComponent*> m_v_ObjectsToTrack;
	AEVec2				  m_x_CurrentViewDistance;
	AEVec2*				  m_px_ScreenSize;
	AEVec2              m_f_DistanceOutTolerance = { 0.05f,0.042f };
	AEVec2              m_f_DistanceInTolerance = { 0.2f,0.2f };
	float					  m_f_ZoomVelocity;
	int					  m_i_CurrentStage = 0;
	bool					  m_b_TrackObjects = true;
	CameraAttributes    m_x_CameraAttributes;

public:
	void AddToTrack(DrawComponent* pDrawObject);
	CameraComponent() = delete;
	CameraComponent(AEVec2* screenSizePtr);
	~CameraComponent();
};


#endif