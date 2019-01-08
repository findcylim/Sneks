#pragma once
#include "DrawObject.h"
#include "SnekHead.h"
class SnekBody : public DrawObject {

public:

	DrawObject* refHead;
	void Update();
};