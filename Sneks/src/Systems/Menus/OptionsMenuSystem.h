#ifndef OPTIONS_MENU_SYSTEM_H
#define OPTIONS_MENU_SYSTEM_H

#include "../../ECS/System.h"
#include "../../Utility/GameStateManager.h"

class OptionsMenuSystem : public BaseSystem
{
	GraphicsSystem* m_o_GraphicsSystem;
	bool fullscreen = true;
public:
	void Initialize(CanvasComponent* canvasComponent);
	void Update(float dt) override;
	OptionsMenuSystem(EntityManager* entityManagerPtr, EventManager* eventManager);
	~OptionsMenuSystem();
	bool ReturnFullscreen();
	void ToggleFullscreen();
};

#endif // !OPTIONS_MENU_SYSTEM_H