
#ifndef CAMERA_H
#define CAMERA_H
#pragma once
#include "DrawObject.h"
#include <vector>
#include "AEVec2.h"

enum CameraState
{
	kCameraZoomIn = 0,
	kCameraZoomOut,
	kCameraIdle
};

struct CameraAttributes
{
	float					  zoomInBaseSpeed    = 0.02f;
	float					  zoomOutBaseSpeed   = 0.01f;
	float					  zoomInMaxSpeed     = 0.25f;
	float					  zoomOutMaxSpeed    = 1.0f;
	float					  numStages          = 2;
	float					  perStageMultiplier = 2.0f;
	float					  speedDecay		    = 0.9f;
};

class Camera
{
private:
	std::vector<DrawObject*> m_v_ObjectsToTrack;
	AEVec2				  m_x_CurrentViewDistance;
	AEVec2*				  m_px_ScreenSize;
	AEVec2              m_f_DistanceOutTolerance = {0.05f,0.05f};
	AEVec2              m_f_DistanceInTolerance  = {0.2f,0.2f};
	float					  m_f_ZoomVelocity;
	int					  m_i_CurrentStage	      = 0;
	CameraAttributes    m_x_CameraAttributes;

public:
	Camera() = delete;
	Camera(AEVec2* screenSizePtr);
	~Camera();
	void AddToTrack(DrawObject* pDrawObject);
	void AutoScroll();
	void Update(float dt);
};
#endif // CAMERA_H
