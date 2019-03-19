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