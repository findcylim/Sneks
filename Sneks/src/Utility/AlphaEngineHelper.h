/* Start Header****************************************************************/
/*!
\file AlphaEngineHelper.h
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains helper functions to some AE Engine features.

\par Contribution : CY - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

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