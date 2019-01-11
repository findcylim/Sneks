#include "SnekBody.h"
#include <cmath>

void SnekBody::Update() {

	//apply the velocity
	AEVec2 test_pos;
	AEVec2FromAngle(&test_pos, m_fRotation);

	m_fPositionX += test_pos.x * m_fVelocity;
	m_fPositionY += test_pos.y * m_fVelocity;

	float body_edge_x = (GetPositionX() - GetRotatedOffsetXX());
	float body_edge_y = (GetPositionY() - GetRotatedOffsetXY());
	float head_edge_x = (refHead->GetPositionX() + refHead->GetRotatedOffsetXX());
	float head_edge_y = (refHead->GetPositionY() + refHead->GetRotatedOffsetXY());

	float diff_x = body_edge_x - head_edge_x;
	float diff_y = body_edge_y - head_edge_y;

	float dist = diff_x * diff_x + diff_y * diff_y;

	auto new_rot = static_cast<float>(atan2(diff_y, diff_x));

	body_edge_x = (GetPositionX() - GetRotatedOffsetXX());
	body_edge_y = (GetPositionY() - GetRotatedOffsetXY());
	head_edge_x = (refHead->GetPositionX() + refHead->GetRotatedOffsetXX());
	head_edge_y = (refHead->GetPositionY() + refHead->GetRotatedOffsetXY());

	diff_x = body_edge_x - head_edge_x;
	diff_y = body_edge_y - head_edge_y;

	//if (newRot < 0)
	//	newRot += 2 * PI;
	//else if (newRot >= 2 * PI)
	//	newRot -= 2 * PI;

	SetRotation(new_rot);

	//cap max distance for calculations at 500
	if (dist > 500 * 500)
		dist = 500 * 500;

	SetVelocity(-fabsf(-3.0f - ((dist / (500 * 500) * 5.0f))));


}