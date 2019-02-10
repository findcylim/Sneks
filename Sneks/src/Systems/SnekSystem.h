#ifndef SNEK_SYSTEM_H
#define SNEK_SYSTEM_H

#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/DrawComponent.h"
#include "GraphicsSystem.h"
#include <vector>
#include "../Components/PhysicsComponent.h"

class SnekSystem final : public BaseSystem // Add event listeners here
{
private:
	GraphicsSystem* m_o_GraphicsSystem;
public:
	SnekSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics);
	~SnekSystem() = default;
	void Update(float dt) override;
	void Initialize();
	void CreateSnek(float posX, float posY, float rotation, const int numBodyParts, const char* textureName, int controlScheme) const;
	void CreateSnekBody(SnekHeadEntity* owner, const char* textureName) const;
	void CreateSnekTail(SnekHeadEntity* owner, const char* textureName) const;
	void CreateMoon(SnekHeadEntity* owner, const char* textureName) const;
	void FaceReference(const TransformComponent* reference, TransformComponent* toChange) const;
	void MoveTowardsReference(::DrawComponent* reference, ::DrawComponent* toChange, PhysicsComponent* headPhysicsComponent) const;
	void CheckOutOfBounds(TransformComponent* transformComponent) const;
	void Flip(SnekHeadEntity* owner);
};
#endif