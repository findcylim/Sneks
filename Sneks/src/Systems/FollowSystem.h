/* Start Header****************************************************************/
/*!
\file FollowSystem.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: XX/XX/2019
\date Modified: XX/XX/2019
\brief This file contains the template to copy paste when creating new systems.

\par Contribution (hours): CY - 0.5

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/
 #pragma once
#include "../ECS/System.h"
#include "../Components/FollowComponent.h"

class FollowSystem : public BaseSystem,
 	public EventListener<Events::EV_PLAYER_COLLISION>
 {
 private:
 	void Receive(const Events::EV_PLAYER_COLLISION& eventData) override;
 public:
 	FollowSystem();
 	~FollowSystem();
 	void Initialize() override;
 	void Update(float dt) override;
   static void FaceReference(const TransformComponent* reference, TransformComponent* toChange);
   void MoveTowardsReference(FollowComponent* follow) const;
 };

