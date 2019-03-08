#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H
#pragma once

#include <vector>
#include "EventManager.h"
#include "System.h"
#include "../Utility/Logger.h"




class SystemManager
{
private:
	std::vector<BaseSystem*> m_v_SystemList;
	EventManager* m_o_EventManager;
	EntityManager* m_o_EntityManager;
	Logger* m_o_Logger;
public:
	size_t GetSystemCount() const;
	void AddSystem(BaseSystem* NewSystem);
	
	void RemoveSystem(BaseSystem* RemSystem);
	void Update(float dt);
	BaseSystem* GetSystem(int ID);
	void Initialize(EventManager* eventManager, EntityManager* entManager);
	SystemManager(Logger* logger);
	~SystemManager();

	//TODO
	int m_i_DroppedFrames;
};

#endif
