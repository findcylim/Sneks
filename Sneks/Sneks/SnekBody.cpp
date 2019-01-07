#include "SnekBody.h"

void SnekBody::Update() {

	/*
	SetVelocity(refHead->GetVelocity());
	float distX = GetPositionX() - refHead->GetPositionX();
	float distY = GetPositionY() - refHead->GetPositionY();
	float dist = sqrt(distX * distX + distY * distY);
	if (dist > 250) {
		float angleDiff = refHead->GetRotation() - GetRotation();
		if (angleDiff < 0)
			angleDiff *= -1;
		if (angleDiff < PI / 18)
			SetRotation(refHead->GetRotation() * 0.1 + GetRotation() * 0.9);
		SetVelocity(GetVelocity() - dist * 0.01);
	}
	*/
	//if out of screen, pop to the other side of screen X
	if (m_fPositionX > AEGfxGetWinMaxX() + m_fSizeX / 2)
		m_fPositionX = AEGfxGetWinMinX() - m_fSizeX / 2;
	else if (m_fPositionX < AEGfxGetWinMinX() - m_fSizeX / 2)
		m_fPositionX = AEGfxGetWinMaxX() + m_fSizeX / 2;

	//if out of screen, pop to the other side of screen Y
	if (m_fPositionY > AEGfxGetWinMaxY() + m_fSizeY / 2)
		m_fPositionY = AEGfxGetWinMinY() - m_fSizeY / 2;
	else if (m_fPositionY < AEGfxGetWinMinY() - m_fSizeY / 2)
		m_fPositionY = AEGfxGetWinMaxY() + m_fSizeY / 2;

	//apply the velocity
	AEVec2 testPos;
	AEVec2FromAngle(&testPos, m_fRotation);


	m_fPositionX += testPos.x * m_fVelocity;
	m_fPositionY += testPos.y * m_fVelocity;
}