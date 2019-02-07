#ifndef SNEK_SYSTEM_H
#define SNEK_SYSTEM_H

#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include <vector>

class SnekSystem final : public BaseSystem // Add event listeners here
{
private:

public:
	SnekSystem(EntityManager* entityManagerPtr);
	~SnekSystem() = default;
	void Update(float dt) override;
	void Initialize();
	void CreateSnek(const int numBodyParts, float posX, float posY,
		AEGfxTexture* snakeHeadTexture, AEGfxTexture* snakeBodyTexture);
};
#endif