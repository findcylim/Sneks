/* Start Header****************************************************************/
/*!
\file TintSystem.cpp
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains the system that tints the snek when it is damaged,
which shows a red tint temporarily.

\par Contribution : CY     - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/
 #pragma once
#include "../ECS/System.h"

class TintSystem : public BaseSystem,
 	public EventListener<Events::EV_PLAYER_COLLISION>
 {
 private:
 	void Receive(const Events::EV_PLAYER_COLLISION& eventData) override;
 public:
 	TintSystem();
 	~TintSystem();
 	void Initialize();
 	void Update(float dt) override;
 };

