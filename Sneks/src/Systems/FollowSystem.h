/* Start Header****************************************************************/
/*!
\file FollowSystem.h
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : CY     - 100.00%

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

