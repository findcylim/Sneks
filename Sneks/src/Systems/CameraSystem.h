#pragma once
#include "../ECS/System.h"
#include "../Math/CameraShake.h"

class CameraSystem : public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>
{
private:
	CameraShake* m_po_CamShake;
	void receive(const Events::EV_PLAYER_COLLISION& eventData) override;
public:

	CameraSystem(EntityManager* entityManagerPtr);
	~CameraSystem();
	void Initialize();
	void UpdateCamera(float dt) const;
	void Update(float dt) override;
	float AddShake(float magnitude);
	void  SetShake(float magnitude);
};

