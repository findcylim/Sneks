#ifndef BACKGROUND_SYSTEM_H
#define BACKGROUND_SYSTEM_H
#pragma once
#include "BackgroundSystem.h"
#include "../ECS/System.h"

class BackgroundSystem :
	public BaseSystem
{
public:
	BackgroundSystem();
	~BackgroundSystem();
};

#endif //BACKGROUND_SYSTEM_H