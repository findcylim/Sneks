#include "SnekBody.h"
#include <math.h>


void SnekBody::Update() {

	float bodyEdgeX = (GetPositionX() - GetRotatedOffsetXX());
	float bodyEdgeY = (GetPositionY() - GetRotatedOffsetXY());
	float headEdgeX = (refHead->GetPositionX() + refHead->GetRotatedOffsetXX());
	float headEdgeY = (refHead->GetPositionY() + refHead->GetRotatedOffsetXY());

	float diffX = -bodyEdgeX + headEdgeX;
	float diffY = -bodyEdgeY + headEdgeY;

	float newRot = atan2(diffY, diffX);
	if (newRot < 0)
		newRot += 2 * PI;

		SetRotation(newRot);


	float dist = diffX * diffX + diffY * diffY;
	
	//cap max distance for calculations at 500
	if (dist > 500 * 500)
		dist = 500 * 500;

	if ( dist < (200 * 200) )
		SetVelocity(0);
	else
		SetVelocity(3.0f +	((dist / (500 * 500) * 5.0f)));


	//apply the velocity
	AEVec2 testPos;
	AEVec2FromAngle(&testPos, m_fRotation);


	m_fPositionX += testPos.x * m_fVelocity;
	m_fPositionY += testPos.y * m_fVelocity;
}