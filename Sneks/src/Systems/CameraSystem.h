/* Start Header ***************************************************************/
/*!
\file CameraSystem.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 5

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once
#include "../ECS/System.h"
#include "../Math/CameraShake.h"

class CameraSystem : public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>
{
private:
	CameraShake* m_po_CamShake;
	void Receive(const Events::EV_PLAYER_COLLISION& eventData) override;
public:

	CameraSystem();
	~CameraSystem();
	void Initialize();
	void UpdateCamera(float dt) const;
	void Update(float dt) override;
	float AddShake(float magnitude);
	void  SetShake(float magnitude);
	void RemoveCameraTrackObjects();
};

