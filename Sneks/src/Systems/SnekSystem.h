#ifndef SNEK_SYSTEM_H
#define SNEK_SYSTEM_H

#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/DrawComponent.h"
#include <vector>

class SnekSystem final : public BaseSystem // Add event listeners here
{
private:

public:
	SnekSystem(EntityManager* entityManagerPtr);
	~SnekSystem() = default;
	void Update(float dt) override;
	void Initialize();
	void CreateSnek(SnekHeadEntity* snekHeadEntity, const int numBodyParts);
	void FaceReference(const TransformComponent* reference, TransformComponent* toChange) const;
	void MoveTowardsReference(const DrawComponent* reference, DrawComponent* toChange) const;
	void CheckOutOfBounds(TransformComponent* transformComponent) const;
};
#endif