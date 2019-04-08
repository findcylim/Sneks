/* Start Header****************************************************************/
/*!
\file GameEntry.cpp
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : CY     - 29.41%  (Equal with the rest + Memory Leak detector)
\par Contribution : Javier - 23.53%  (Equal with the rest)
\par Contribution : Edwin  - 23.53%  (Equal with the rest)
\par Contribution : Adam   - 23.53%  (Equal with the rest)

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "ECS/EventManager.h"
#include "ECS/ECSystem.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE,
	LPSTR, int nCmdShow)
{
	//NOTE:: VS check for mem leaks use this before submission
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	AlphaEngineHelper::AlphaInitialize(hInstance, nCmdShow);
	ECSystem* engine = new ECSystem;
	engine->InitializeEngine();

	while (engine->IsEngineOn())
	{
		engine->Update();

	}
	delete(engine);
	AESysExit();
	//assert(!LogMemoryLeaks() && "Memory leaks detected! Check log file for more info.");
	return 0;
}