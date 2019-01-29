#include "SnekBody.h"
#include <cmath>
#include <iostream>

SnekBody::SnekBody(float posX, float posY, float sizeX, float sizeY, AEGfxTexture* tex, DrawObject* reference)
	:DrawObject(posX, posY, sizeX, sizeY, tex)
{
	m_o_Reference = reference;
	//Set the position to behind the reference, after rotation

	//SetPositionX(GetRotatedOffsetXx() + GetPosition().x());
	//SetPositionY(GetRotatedOffsetXy() + GetPosition().y());
	//SetPositionX(100.0f);

	//FaceReference();
}

float referenceHalfSizeSquared = 0;
void SnekBody::Update(float dt) 
{


	float distanceX, distanceY;
	FaceReference(&distanceX, &distanceY);

	float distanceXySquared = distanceX * distanceX + distanceY * distanceY;


	if (fabsf(distanceX) > m_o_Reference->GetSizeX() * m_o_Reference->GetScale() / 2 || 1) {
		float targetPosX = m_o_Reference->GetPosition().x + distanceX * 0.7f;
		float targetPosY = m_o_Reference->GetPosition().y + distanceY * 0.7f;

		SetPosition(targetPosX, targetPosY);
	}


	m_f_Scale = m_o_Reference->GetScale();
	/*float distanceXySquared = distanceX * distanceX + distanceY * distanceY;

	//cap max distance for speed calculations at 500
	if (distanceXySquared > 60 * 60)
		distanceXySquared = 60 * 60;
	if (referenceHalfSizeSquared== 0)
		float referenceHalfSizeSquared = (m_o_Reference->GetSizeX() * m_o_Reference->GetScale() / 2) * 
													(m_o_Reference->GetSizeX() * m_o_Reference->GetScale() / 2);

	
	if (distanceXySquared < 30*30)
	{
		SetVelocity(GetVelocity()*0.5f);
	}
	if (distanceXySquared < referenceHalfSizeSquared) {
		SetVelocity(0);
		SetPosition();
	}
	else //move towards the reference
		SetVelocity(-fabsf(-0.0f - ((distanceXySquared - referenceHalfSizeSquared * 0.8f) * 0.002f)));

	*/

	//apply the velocity
	//ApplyVelocity(dt);
	/*AEVec2 directionVector;
	AEVec2FromAngle(&directionVector, m_f_Rotation);

	m_x_Position.x += directionVector.x * m_f_Velocity * 300 * dt;
	m_x_Position.y += directionVector.y * m_f_Velocity * 300 * dt;*/
}

void SnekBody::FaceReference()
{
	float f;
	FaceReference(&f, &f);
}

void SnekBody::FaceReference(float* retDistanceX, float* retDistanceY)
{
	float referenceEdgeX = (m_o_Reference->GetPosition().x);// +refHead->GetRotatedOffsetXx());
	float referenceEdgeY = (m_o_Reference->GetPosition().y);// +refHead->GetRotatedOffsetXy());

	float distanceX = GetPosition().x- referenceEdgeX;
	float distanceY = GetPosition().y- referenceEdgeY;

	auto newRot = static_cast<float>(atan2(distanceY, distanceX));
	// make sure rotation is between 0 and 2Pi
	if (newRot < 0)
		newRot += 2 * PI;
	else if (newRot >= 2 * PI)
		newRot -= 2 * PI;

	SetRotation(newRot);

	*retDistanceX = GetPosition().x- referenceEdgeX;
	*retDistanceY = GetPosition().y- referenceEdgeY;
}

SnekBody::~SnekBody() = default;
