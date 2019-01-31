#include "Buildings.h"


//TODO: REPLACE THESE ARE TEMP VARIABLES
constexpr int buildingsDistX = 80;
constexpr int buildingsDistY = 51;

Buildings::Buildings(const int bgInstancesX, const int bgInstancesY, AEGfxTexture* buildingTexture)
{
	m_BuildingsTexture = buildingTexture;
	m_px_BuildingMesh = nullptr;

	m_i_MaxBuildingsX = 1920 / 80 * (bgInstancesX * 2 + 1);
	m_i_MaxBuildingsY = 1080 / 51 * (bgInstancesY * 2 + 1);

	//The coordinates of the building at the origin
	m_i_FirstBuildingCoords.x = -1920.0f * bgInstancesX -35.5f -buildingsDistX * 11.0f;
	m_i_FirstBuildingCoords.y = -1080.0f * bgInstancesY -buildingsDistY * 10.0f;

	m_BuildingInstances.clear();
	m_BuildingCoordsCurrent.clear();

	LoadPossibleLocations();
	GenerateNewBuildings(100);
}


Buildings::~Buildings() = default;

void Buildings::Draw()
{
	for (auto& i_Buildings : m_BuildingInstances) {
		i_Buildings->Draw();
	}
}

void Buildings::Update(float dt)
{
}

void Buildings::GenerateNewBuildings(int num)
{
	for (int i = 0; i < num; i++) {
		auto newPosition = GetNewUniqueBuildingPosition();
		if (!newPosition)
			return;
		m_BuildingCoordsCurrent.push_back(*newPosition);
		DrawObject* building;
		if (m_px_BuildingMesh == nullptr) {
			building = new DrawObject(newPosition->x, newPosition->y, 71, 42, m_BuildingsTexture);
			m_px_BuildingMesh = building->GetMesh();
		}else
		{
			building = new DrawObject(newPosition->x, newPosition->y, 71, 42, m_BuildingsTexture,m_px_BuildingMesh);
		}
		m_BuildingInstances.push_back(building);
		//ELSE: ALL BUILDINGS BUILT
	}
}

void Buildings::RemoveBuildings()
{
	m_BuildingInstances.clear();
	m_BuildingCoordsCurrent.clear();
}

void Buildings::LoadPossibleLocations()
{
	//TODO: LOAD BUILDING COORDS BY FILE INPUT ( LEVEL EDITOR )
	for (int i_MaxBuildingsX = 0; i_MaxBuildingsX < m_i_MaxBuildingsX; i_MaxBuildingsX++)
	{
		for (int i_MaxBuildingsY = 0; i_MaxBuildingsY < m_i_MaxBuildingsY; i_MaxBuildingsY++)
		{
			HTVector2 possibleBuildingCoord{ m_i_FirstBuildingCoords.x + i_MaxBuildingsX * buildingsDistX,
														 m_i_FirstBuildingCoords.y + i_MaxBuildingsY * buildingsDistY };
			m_BuildingCoordsPossible.push_back(possibleBuildingCoord);
		}
	}
}

HTVector2* Buildings::GetNewUniqueBuildingPosition()
{
	if (m_BuildingCoordsCurrent.size() < m_BuildingCoordsPossible.size()) {
		int randIndex = 0;
		HTVector2* currentBuildingCoords = 0;
		bool uniqueIndex = false;
		while (!uniqueIndex) {
			uniqueIndex = true;
			//Get a random index at possible buildings
			randIndex = static_cast<int>(rand() % m_BuildingCoordsPossible.size());
			currentBuildingCoords = &m_BuildingCoordsPossible.at(randIndex);
			for (auto& i_Built : m_BuildingCoordsCurrent) {
				//Check for any m_x_Current buildings with the same coordinates
				if (i_Built.x == currentBuildingCoords->x && i_Built.y == currentBuildingCoords->y)
				{
					uniqueIndex = false;
				}
			}
		}
		return currentBuildingCoords;
	}
	return nullptr;
}