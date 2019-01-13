#ifndef SNEK_BODY_H
#define SNEK_BODY_H
#pragma once

#include "DrawObject.h"
#include "SnekHead.h"

class SnekBody : public DrawObject {
public:
	SnekBody(float posX, float posY, float sizeX, float sizeY, AEGfxTexture* tex, DrawObject* reference);
	DrawObject* m_o_Reference; // the DrawObject that this body is following
	void Update();
	// change the rotation to face the reference object
	void FaceReference();
	// change the rotation to face the reference object, returns the diff in distance
	void FaceReference(float* distanceX, float* distanceY);
	~SnekBody();
};
#endif //SNEK_BODY_H