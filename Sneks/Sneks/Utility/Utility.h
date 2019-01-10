#pragma once

#include "Logger.h"
#include "GameStateManager.h"




class Utility
{
public:
	Logger* m_Logger;
	GameStateManager* m_GSManager;

	//There should only be one instance per Window
	//Limit your news
	Utility();
	~Utility();
};



