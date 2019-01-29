#ifndef BUILDINGS_H
#define BUILDINGS_H
#pragma once
#include "DrawObject.h"
#include <vector>
#include "../Math/HTVector2.h"

class Buildings
{
private:
	std::vector<DrawObject*> m_BuildingInstances = {};
	std::vector<HTVector2>   m_BuildingCoordsCurrent;
	std::vector<HTVector2>   m_BuildingCoordsPossible;
	int m_i_MaxBuildingsX;
	int m_i_MaxBuildingsY;
	HTVector2 m_i_FirstBuildingCoords;
	AEGfxTexture* m_BuildingsTexture;

public:
	Buildings(int instancesX, int instancesY, AEGfxTexture* bgTexture);
	Buildings() = delete;
	~Buildings();
	void Draw();
	void Update(float dt);
	void GenerateNewBuildings(int num);
	void RemoveBuildings();
	void LoadPossibleLocations();
	HTVector2* GetNewUniqueBuildingPosition();
};

#endif // BUILDINGS_H