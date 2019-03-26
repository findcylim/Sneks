/* Start Header ***************************************************************/
/*!
\file BuildingsSystem.h
\author Lim Chu Yan, chuyan.lim, 440002918
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 5

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef BUILDINGS_SYSTEM_H
#define BUILDINGS_SYSTEM_H
#pragma once
#include "../ECS/System.h"
#include "GraphicsSystem.h"

class BuildingsSystem:
	public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>
{
public:
	BuildingsSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics);
	void Receive(const Events::EV_PLAYER_COLLISION& eventData) override;

	void Update(float dt) override;
	void Initialize();
	void ResetLevel1();
	StaticObjectEntity* CreateBuilding(float posX, float posY, const char* textureName, HTVector2 scale) const;

	~BuildingsSystem();
	void GenerateNewBuildings(int num);
	void RemoveBuildings();
	void LoadPossibleLocations();
	HTVector2* GetNewUniqueBuildingPosition(HTVector2& scale);

private:
	GraphicsSystem*                  m_o_GraphicsSystem;
	std::vector<StaticObjectEntity*> m_BuildingInstances ={};
	std::vector<HTVector2>           m_BuildingCoordsCurrent;
	std::vector<HTVector2>           m_BuildingCoordsPossible;
	int                              m_i_MaxBuildingsX;
	int                              m_i_MaxBuildingsY;
	HTVector2                        m_i_FirstBuildingCoords;
	int m_i_BuildingsCount = 0;
	const int kBuildingsSpawnCount = 1000;
};

#endif //BUILDINGS_SYSTEM_H