/* Start Header****************************************************************/
/*!
\file CanvasUISystem.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Adam   - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once
#include "../ECS/System.h"
#include "GraphicsSystem.h"
class CanvasUISystem :
	public BaseSystem,
	public EventListener<Events::EV_NEW_UI_ELEMENT>,
	public EventListener<Events::EV_PLAYER_COLLISION>
{
	GraphicsSystem* m_po_GraphicsManager;
	HTVector2 m_o_ScreenSize;
	bool m_b_IsMousePressed = false;
public:
	void Update(float dt) override;
	CanvasUISystem(GraphicsSystem* graphicsManager);
	~CanvasUISystem();
	void Initialize();
	void AddElement(CanvasComponent* canvasComponent, HTVector2 initialOffset, CanvasElementEnum num, const char * name,
		const char * uiElementSprite, const char* uiText = "", const char * uiHoverSprite = "", const char * uiClickSprite = "", 
		void(*ButtonFunction)(SystemManager*) = nullptr, HTColor textColor = {1.0f,1.0f,1.0f,1.0f});
	void ClearUI(CanvasComponent* canvas);
	void Receive(const Events::EV_NEW_UI_ELEMENT& eventData) override;
	void Receive(const Events::EV_PLAYER_COLLISION& eventData) override;
};
