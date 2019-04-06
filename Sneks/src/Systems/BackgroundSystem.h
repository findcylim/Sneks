/* Start Header****************************************************************/
/*!
\file BackgroundSystem.h
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : CY     - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef BACKGROUND_SYSTEM_H
#define BACKGROUND_SYSTEM_H
#pragma once
#include "BackgroundSystem.h"
#include "../ECS/System.h"
#include "GraphicsSystem.h"

class BackgroundSystem :
	public BaseSystem
{
public:
	BackgroundSystem( GraphicsSystem* graphics);
	void Update(float dt);
	void Initialize();
	BackgroundEntity* CreateBackground(float posX, float posY, float sizeX, float sizeY, const char* textureName) const;
	void CreateInstancedBackgrounds(int instancesX, int instancesY, const char* textureName);
	~BackgroundSystem() = default;

private:
	GraphicsSystem* m_o_GraphicsSystem;
};

#endif //BACKGROUND_SYSTEM_H