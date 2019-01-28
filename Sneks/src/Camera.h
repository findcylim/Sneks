
#ifndef CAMERA_H
#define CAMERA_H
#pragma once
#include "DrawObject.h"
#include <vector>
#include "AEVec2.h"

class Camera
{
private:
	std::vector<DrawObject*> m_v_ObjectsToTrack;
	AEVec2				  m_x_CurrentViewDistance;
	AEVec2*				  m_px_ScreenSize;
	AEVec2              m_f_DistanceOutTolerance ={0.05f,0.05f};
	AEVec2              m_f_DistanceInTolerance = {0.2f,0.2f};
	float					  m_f_ZoomOutVelocity;

	float					  m_f_cameraBaseSpeed;
	float					  m_f_cameraAcceleration;
	float					  m_f_cameraPerStageMultiplier;


public:
	Camera() = delete;
	Camera(AEVec2* screenSizePtr);
	~Camera();
	void AddToTrack(DrawObject* pDrawObject);
	void AutoScroll();
	void Update(float dt);
};
#endif // CAMERA_H
