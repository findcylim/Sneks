#include "SnekBody.h"
#include <cmath>

SnekBody::SnekBody(float posX, float posY, float sizeX, float sizeY, AEGfxTexture* tex, DrawObject* reference)
	:DrawObject(posX, posY, sizeX, sizeY, tex)
{
	m_o_Reference = reference;
	//Set the position to behind the reference, after rotation

	//SetPositionX(GetRotatedOffsetXx() + GetPositionX());
	//SetPositionY(GetRotatedOffsetXy() + GetPositionY());
	//SetPositionX(100.0f);

	//FaceReference();
}

void SnekBody::Update() {
	//apply the velocity
	AEVec2 directionVector;
	AEVec2FromAngle(&directionVector, m_f_Rotation);

	m_f_PositionX += directionVector.x * m_f_Velocity;
	m_f_PositionY += directionVector.y * m_f_Velocity;

	float distanceX, distanceY;
	FaceReference(&distanceX, &distanceY);

	float distanceXySquared = distanceX * distanceX + distanceY * distanceY;

	//cap max distance for speed calculations at 500
	if (distanceXySquared > 300 * 300)
		distanceXySquared = 300 * 300;
	if (distanceXySquared < (m_o_Reference->GetSizeX() / 2) * (m_o_Reference->GetSizeX() / 2))
		SetVelocity(0);
	else //move towards the reference
		SetVelocity(-fabsf(-3.0f - ((distanceXySquared / (300 * 300) * 50.0f))));
}

void SnekBody::FaceReference()
{
	float f;
	FaceReference(&f, &f);
}

void SnekBody::FaceReference(float* retDistanceX, float* retDistanceY)
{
	float referenceEdgeX = (m_o_Reference->GetPositionX());// +refHead->GetRotatedOffsetXx());
	float referenceEdgeY = (m_o_Reference->GetPositionY());// +refHead->GetRotatedOffsetXy());

	float distanceX = GetPositionX() - referenceEdgeX;
	float distanceY = GetPositionY() - referenceEdgeY;

	auto newRot = static_cast<float>(atan2(distanceY, distanceX));
	// make sure rotation is between 0 and 2Pi
	if (newRot < 0)
		newRot += 2 * PI;
	else if (newRot >= 2 * PI)
		newRot -= 2 * PI;

	SetRotation(newRot);

	*retDistanceX = GetPositionX() - referenceEdgeX;
	*retDistanceY = GetPositionY() - referenceEdgeY;
}

SnekBody::~SnekBody()
{
	delete this;
}