#ifndef BUILDINGS_SYSTEM_H
#define BUILDINGS_SYSTEM_H
#pragma once
#include "../ECS/System.h"
#include "GraphicsSystem.h"

class BuildingsSystem:
	public BaseSystem
{
public:
	BuildingsSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics);
	void Update(float dt) override;
	void Initialize();
	StaticObjectEntity* CreateBuilding(float posX, float posY, float sizeX, float sizeY, const char* textureName) const;
	void CreateInstancedBackgrounds(int instancesX, int instancesY, const char* textureName);

	~BuildingsSystem() = default;
	void GenerateNewBuildings(int num);
	void RemoveBuildings();
	void LoadPossibleLocations();
	HTVector2* GetNewUniqueBuildingPosition();

private:
	GraphicsSystem*                  m_o_GraphicsSystem;
	std::vector<StaticObjectEntity*> m_BuildingInstances ={};
	std::vector<HTVector2>           m_BuildingCoordsCurrent;
	std::vector<HTVector2>           m_BuildingCoordsPossible;
	int                              m_i_MaxBuildingsX;
	int                              m_i_MaxBuildingsY;
	HTVector2                        m_i_FirstBuildingCoords;

};

#endif //BUILDINGS_SYSTEM_H