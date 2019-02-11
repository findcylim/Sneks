#include "BuildingsSystem.h"
#include "../Components/CollisionComponent.h"

//TODO: REPLACE THESE ARE TEMP VARIABLES
constexpr int buildingsDistX = 80;
constexpr int buildingsDistY = 51;
constexpr int bgInstancesX = 0;
constexpr int bgInstancesY = 0;


BuildingsSystem::BuildingsSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics) :
	BaseSystem(entityManagerPtr)
{
	m_o_GraphicsSystem = graphics;
}


void BuildingsSystem::Update(float dt)
{

}

void BuildingsSystem::Initialize()
{
	m_i_MaxBuildingsX = 1920 / 80 * (bgInstancesX * 2 + 1);
	m_i_MaxBuildingsY = 1080 / 51 * (bgInstancesY * 2 + 1);

	//The coordinates of the building at the origin
	m_i_FirstBuildingCoords.x = -1920.0f * bgInstancesX - 35.5f - buildingsDistX * 11.0f;
	m_i_FirstBuildingCoords.y = -1080.0f * bgInstancesY - buildingsDistY * 10.0f;

	m_BuildingInstances.clear();
	m_BuildingCoordsCurrent.clear();

	LoadPossibleLocations();
	GenerateNewBuildings(100);

}

StaticObjectEntity* BuildingsSystem::CreateBuilding(float posX, float posY, float sizeX, float sizeY,
                                                    const char* textureName) const
{
	auto newBuildingEntity = static_cast<StaticObjectEntity*>(
		m_po_EntityManager->NewEntity(kEntityStaticObject, "Building"));

	for (auto i_Component : newBuildingEntity->m_v_AttachedComponentsList)
	{
		if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			static_cast<TransformComponent*>(i_Component)->SetPosition(posX, posY);
			static_cast<TransformComponent*>(i_Component)->SetRotation(0);
		}
		else if (i_Component->m_x_ComponentID == kComponentDraw)
		{
			static_cast<DrawComponent*>(i_Component)->Initialize(
				m_o_GraphicsSystem->FetchTexture(textureName),
				sizeX, sizeY, HTColor{ 1,1,1,1 }
			);
			static_cast<DrawComponent*>(i_Component)->m_f_DrawPriority = 9;
		}
		else if (i_Component->m_x_ComponentID == kComponentCollision)
		{
			static_cast<CollisionComponent*>(i_Component)->m_i_CollisionGroupVec.push_back
			(kCollGroupBuilding);
		}
	}
	return newBuildingEntity;
}

void BuildingsSystem::GenerateNewBuildings(int num)
{
	for (int i = 0; i < num; i++) {
		auto newPosition = GetNewUniqueBuildingPosition();
		if (!newPosition)
			return;
		m_BuildingCoordsCurrent.push_back(*newPosition);

		
		m_BuildingInstances.push_back(
			CreateBuilding(newPosition->x, newPosition->y, 71, 42, "Building01")
		);
		/*DrawObject* building;
		if (m_px_BuildingMesh == nullptr) {
			building = new DrawObject(newPosition->x, newPosition->y, 71, 42, m_BuildingsTexture);
			m_px_BuildingMesh = building->GetMesh();
		}
		else
		{
			building = new DrawObject(newPosition->x, newPosition->y, 71, 42, m_BuildingsTexture, m_px_BuildingMesh);
		}
		m_BuildingInstances.push_back(building);*/
		//ELSE: ALL BUILDINGS BUILT
	}
}

void BuildingsSystem::RemoveBuildings()
{
	for (auto buildingInstance: m_BuildingInstances)
	{
		m_po_EntityManager->DeleteEntity(buildingInstance);
	}
	m_BuildingInstances.clear();
	m_BuildingCoordsCurrent.clear();
}

void BuildingsSystem::LoadPossibleLocations()
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

HTVector2* BuildingsSystem::GetNewUniqueBuildingPosition()
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
