#pragma once
#include "../ECS/System.h"
class CanvasUISystem :
	public BaseSystem
{
public:
	virtual void Update(float dt) override;
	CanvasUISystem(EntityManager* entityManagerPtr);
	~CanvasUISystem();
	void Initialize();
	void AddElement(HTVector2 initPosition, CanvasElementEnum num, const char * name);

};

