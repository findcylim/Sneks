/* Start Header****************************************************************/
/*!
\file TutorialMenuSystem.h
\author Primary Author : Javier Foo, javier.foo
\par email: javier.foo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Javier     - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef TUTORIAL_MENU_SYSTEM_H
#define TUTORIAL_MENU_SYSTEM_H


#include "../../ECS/System.h"
#include "../../Utility/GameStateManager.h"

class TutorialMenuSystem :
	public BaseSystem,
	public EventListener<Events::EV_PLAYER_MOVEMENT_KEY>
{
	GameStateManager* m_po_GameStateManager;
	float timer; // for enabling display after set amount of time
public:
	/* Constructor/Destructor */
	TutorialMenuSystem(GameStateManager* gameStateManager);
	~TutorialMenuSystem();

	/* General System Functions */
	void Initialize();
	void Receive(const Events::EV_PLAYER_MOVEMENT_KEY& eventData) override;
	void Update(float dt)override;
};

#endif // !TUTORIAL_MENU_SYSTEM_H
