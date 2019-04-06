/* Start Header****************************************************************/
/*!
\file EventManager.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : CY     - 11.11%  (Event Structs)
\par Contribution : Javier - 11.11%  (Event Structs)
\par Contribution : Edwin  - 11.11%  (Event Structs)
\par Contribution : Adam   - 66.67%  (EventManager struct and management)

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/


#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <list>
#include <vector>
#include <map>
#include "../Utility/Logger.h"
#include "EventListener.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/SnekHeadComponent.h"
#include "../Components/CanvasComponent.h"
#include "../Components/TextRendererComponent.h"
#include "../Utility/RTTIHelper.h"

enum State : int;

enum kEventList
{
	Ev_PLAYER1GAME_LEFTKEY,
	Ev_PLAYER1GAME_RIGHTKEY,
	Ev_PLAYER1GAME_LEFTSHIFTKEY,
	Ev_PLAYER2GAME_LEFTKEY,
	Ev_PLAYER2GAME_RIGHTKEY,
	Ev_PLAYER2GAME_RIGHTSHIFTKEY,

};


namespace Events
{
	//Declare all events here
	enum MoveKey
	{
		MOVE_KEY_LEFT = 0,
		MOVE_KEY_RIGHT,
		MOVE_KEY_UP,
		MOVE_KEY_DOWN
	};

	struct EV_PLAYER_MOVEMENT_KEY final
	{
		PhysicsComponent* caller;
		MoveKey key;
	};

	struct EV_PLAYER_COLLISION final
	{
		CollisionComponent* object1;
		CollisionComponent* object2;
	};

	struct EV_ENTITY_POOL_CHANGED final
	{

	};

	struct EV_CREATE_PROJECTILE
	{
		bool isCollide;
		float rot, speed, scale;
		HTVector2* pos, *velocity;
		const char * texName;
	};

	struct EV_NEW_UI_ELEMENT
	{
		CanvasComponent* canvas;
		HTVector2 initialPosition;
		CanvasElementEnum elementType;
		const char * elementEntityName;
		const char * uiElementSpriteName;
		const char * uiTextLabel = "";
		const char * uiHoverSpriteName = "";
		const char * uiClickSpriteName = "";
		void(*ButtonPressFunc)(SystemManager*) = nullptr;
		HTColor textColor = { 0.0f,0.0f, 0.0f, 0.0f };
	};

	struct EV_PLAYER_COLLISION_ON_ENTER
	{
		CollisionComponent* object1;
	};

	struct EV_PLAYER_COLLISION_ON_EXIT
	{
		CollisionComponent* object1;
	};

	struct EV_MOUSE_ONCLICK
	{

	};

	struct EV_PLAYER_WIN
	{
		char WinnerNumber;
	};

	struct EV_GAME_STATE_CHANGED
	{
		State changedToState;
		State changedFromState;
	};

	struct EV_POWERUP_PICKUP_SPEED
	{
	};

	struct EV_POWERUP_PICKUP_HEALTH
	{
	};

	struct EV_POWERUP_PICKUP_STARMODE
	{
	};

	struct EV_SPECIAL_SKILL_BOOST
	{
	};

	struct EV_SPECIAL_SKILL_FLIP
	{
	};
	struct EV_PAUSED_GAME
	{
		bool pauseCheck;
	};
}

class EventManager
{
protected:
public:
	void Update() {}
	void ProcessEvents() {}


	template<typename T>
	bool AddListener(EventListener<T>* listener, BaseSystem* systemPtr)
	{
		auto type = getTypeIndex<T>();
		auto v_Listener = m_l_ListenerList.find(type);

		if (v_Listener != m_l_ListenerList.end())
		{
			listener->systemListener = systemPtr;
			v_Listener->second.push_back(listener);
		}
		else
		{
			std::vector<BaseEventListener*> v_newEvent;
			listener->systemListener = systemPtr;
			v_newEvent.push_back(listener);
			m_l_ListenerList.insert({ type,v_newEvent });
		}
		return true;
	}

	template<typename T>
	void RemoveListener(EventListener<T>* listener)
	{
		auto type = getTypeIndex<T>();
		auto v_Listener = m_l_ListenerList.find(type);
		for (auto i_Listener = v_Listener->second.begin(); i_Listener <= v_Listener->second.end(); ++i_Listener)
		{
			auto ii_Listener = reinterpret_cast<BaseEventListener*>(listener);
			if (ii_Listener == *i_Listener)
			{
				v_Listener->second.erase(i_Listener);
				return;
			}
		}
	}

	template<typename T>
	void RemoveAllListener()
	{
		auto type = getTypeIndex<T>();
		auto v_Listener = m_l_ListenerList.find(type);
		if (v_Listener != m_l_ListenerList.end())
		{
			auto* i_Listener = v_Listener->second.begin();
			while (i_Listener != v_Listener->second.end())
			{
				v_Listener->second.erase(i_Listener);
			}
		}
	}

	template<typename T>
	bool EmitEvent(const T& event)
	{
		auto i_Listener = m_l_ListenerList.find(getTypeIndex<T>());
		if (i_Listener != m_l_ListenerList.end())
		{
			for (auto* baseListener : i_Listener->second)
			{
				auto* Listener = reinterpret_cast<EventListener<T>*>(baseListener);
				if(Listener->systemListener->m_b_isActive)
					Listener->Receive(event);
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	EventManager() {}
	virtual ~EventManager() {}
private:
	std::map<std::type_index, std::vector<BaseEventListener*>> m_l_ListenerList;
};

#endif