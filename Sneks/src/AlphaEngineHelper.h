#pragma once
#include <minwindef.h> //HINSTANCE typdef

namespace AlphaEngineHelper
{
	void AlphaInitialize(HINSTANCE hInstance, int nCmdShow);
	void GetScreenSize(float* retWidth, float* retHeight);

};

