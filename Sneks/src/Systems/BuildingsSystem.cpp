#include "BuildingsSystem.h"
#include "../Components/CollisionComponent.h"

//TODO: REPLACE THESE ARE TEMP VARIABLES
//TODO: INVESTIGATE LAG DUE TO INCREASE IN BUILDINGS (in pascal coms)
//TODO: analyse the code and make the buildings cover the entire map
constexpr int buildingsDistX = 80;
constexpr int buildingsDistY = 45;
constexpr int bgInstancesX = 2;
constexpr int bgInstancesY = 2;


BuildingsSystem::BuildingsSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics) :
	BaseSystem(entityManagerPtr)
{
	m_o_GraphicsSystem = graphics;
}


void BuildingsSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	/*
	test for object to follow camera center
	works now
	todo: implement the moving of objects together with normal culling
	should move when camera touches the edge of each background

	auto bg = m_po_EntityManager->GetFirstEntityInstance<StaticObjectEntity>(kEntityStaticObject);
	auto camera = m_po_EntityManager->GetFirstEntityInstance<CameraEntity>(kEntityCamera);
	bg->GetComponent<TransformComponent>()->m_x_Position = HTVector2{ 0,0 }-camera->GetComponent<CameraComponent>()->m_f_VirtualOffset;
	*/
}

void BuildingsSystem::Initialize()
{
	m_i_MaxBuildingsX = 1920 / 80 * (bgInstancesX * 2 + 1);
	m_i_MaxBuildingsY = 1080 / 45 * (bgInstancesY * 2 + 1);

	//The coordinates of the building at the origin
	m_i_FirstBuildingCoords.x = -1920.0f * (bgInstancesX + 0.5f) - 33.5f - buildingsDistX - 13.0f + 160;
	m_i_FirstBuildingCoords.y = -1080.0f * (bgInstancesY + 0.5f) - 21.0f - buildingsDistY + 6.0f + 90;

	m_BuildingInstances.clear();
	m_BuildingCoordsCurrent.clear();

	LoadPossibleLocations();
	GenerateNewBuildings(1500);

}

StaticObjectEntity* BuildingsSystem::CreateBuilding(float posX, float posY, 
                                                    const char* textureName,HTVector2 scale) const
{
	
	auto newBuildingEntity =
		m_po_EntityManager->NewEntity<StaticObjectEntity>(kEntityStaticObject, "Building");

	for (auto i_Component : newBuildingEntity->m_v_AttachedComponentsList)
	{
		if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			static_cast<TransformComponent*>(i_Component)->SetPositionX(posX);
			static_cast<TransformComponent*>(i_Component)->SetPositionY(posY);
			static_cast<TransformComponent*>(i_Component)->SetRotation(0);
			static_cast<TransformComponent*>(i_Component)->m_f_Scale.x *= scale.x;
			static_cast<TransformComponent*>(i_Component)->m_f_Scale.y *= scale.y;
		}
		else if (i_Component->m_x_ComponentID == kComponentDraw)
		{
			m_o_GraphicsSystem->InitializeDrawComponent(static_cast<DrawComponent*>(i_Component), textureName);
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
		HTVector2 scale = { 1,1 };
		HTVector2* newPosition = GetNewUniqueBuildingPosition(scale);
		if (!newPosition)
			return;
		int offset = 1;
		if (scale.x != 1)
		{
			offset = 2;
		}
		
		const char * buildingName = "Building01";
		int check = (rand() % 100)/5;
		if (check >= 0 && check <= 7)
		{
			buildingName = "Building01";
		}
		else if (check >= 8 && check <= 14)
		{
			buildingName = "Building02";
		}
		else if (check >= 15 && check <= 18)
		{
			buildingName = "Building03";
		}
		else 
		{
			buildingName = "Building04";
		}

		m_BuildingInstances.push_back(
			CreateBuilding(newPosition->x + (offset - 1)* (buildingsDistX*0.5f),
						   newPosition->y + (offset - 1)* (buildingsDistY*0.5f), buildingName, scale)
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
		m_po_EntityManager->AddToDeleteQueue(buildingInstance);
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

HTVector2* BuildingsSystem::GetNewUniqueBuildingPosition(HTVector2& scale)
{
	if (m_BuildingCoordsCurrent.size() < m_BuildingCoordsPossible.size()) {
		int randIndex = 0;
		HTVector2* currentBuildingCoords = 0;
		bool uniqueIndex = false;
		int check = (rand() % 100)/25;
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
				if (check == 1)
				{
					if (i_Built.x == (currentBuildingCoords->x + buildingsDistX) && i_Built.y == currentBuildingCoords->y)
					{
						uniqueIndex = false;
					}
					else if (i_Built.x == currentBuildingCoords->x && i_Built.y == (currentBuildingCoords->y + buildingsDistY))
					{
						uniqueIndex = false;
					}
					else if (i_Built.x == (currentBuildingCoords->x + buildingsDistX) && i_Built.y == (currentBuildingCoords->y + buildingsDistY))
					{
						uniqueIndex = false;
					}
				}
			}
		}
		
		if (currentBuildingCoords)
		{
			switch (check)
			{
			case 0:
			//TODO
			// ADD UI COLLISION SEPERATE FROM MAIN COLLISION
			case 3:
				m_BuildingCoordsCurrent.push_back(*currentBuildingCoords);
				break;
			case 1:
				m_BuildingCoordsCurrent.push_back(*currentBuildingCoords);
				m_BuildingCoordsCurrent.push_back(HTVector2{ ((*currentBuildingCoords).x + buildingsDistX),(*currentBuildingCoords).y });
				m_BuildingCoordsCurrent.push_back(HTVector2{ (*currentBuildingCoords).x,  ((*currentBuildingCoords).y + buildingsDistY) });
				m_BuildingCoordsCurrent.push_back(HTVector2{ ((*currentBuildingCoords).x + buildingsDistX),((*currentBuildingCoords).y + buildingsDistY) });
				scale.x = 2.18f;
				scale.y = 2.45f;
				break;
			}
		}
		return currentBuildingCoords;
	}
	return nullptr;
}
