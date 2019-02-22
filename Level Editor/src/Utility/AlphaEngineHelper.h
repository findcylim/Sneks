#ifndef ALPHA_ENGINE_HELPER_H
#define ALPHA_ENGINE_HELPER_H
#pragma once
#include <windows.h>

namespace AlphaEngineHelper
{
	void AlphaInitialize(HINSTANCE hInstance, int nCmdShow);
	void GetScreenSize(float* retWidth, float* retHeight);

};

#endif