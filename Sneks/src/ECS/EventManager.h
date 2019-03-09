
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
#include "../Utility/RTTIHelper.h"


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

	struct IEvent
	{
		virtual ~IEvent() {}
	};

	struct IEventWrapper
	{
		
	};

	struct EV_PLAYER_MOVEMENT_KEY final : public IEvent
	{
		EV_PLAYER_MOVEMENT_KEY() {}
		EV_PLAYER_MOVEMENT_KEY(PhysicsComponent* _caller, MoveKey _key) 
							   :IEvent{}, caller{ _caller }, key{_key} {}
		PhysicsComponent* caller;
		MoveKey key;
	};

	struct EV_PLAYER_COLLISION final : public IEvent
	{
		EV_PLAYER_COLLISION() {}
		EV_PLAYER_COLLISION(CollisionComponent* obj1, CollisionComponent* obj2) 
							:IEvent{ }, object1{obj1}, object2 { obj2 }{}
		CollisionComponent* object1;
		CollisionComponent* object2;
	};

	struct EV_ENTITY_POOL_CHANGED final : public IEvent
	{
		EV_ENTITY_POOL_CHANGED() :IEvent{} {}
	};

	struct EV_SNEK_INVULNERABLE : public IEvent
	{
		EV_SNEK_INVULNERABLE() {}
		EV_SNEK_INVULNERABLE(SnekHeadComponent* _snekHead) :IEvent{}, snekHead{_snekHead} {}
		SnekHeadComponent* snekHead;
	};

	struct EV_CREATE_PROJECTILE : public IEvent
	{
		EV_CREATE_PROJECTILE() {};
		EV_CREATE_PROJECTILE(bool _isCollide, float _rot,
			float _speed, float _scale,
			HTVector2* _pos, HTVector2 *_velocity,
			const char * _texName) 
			:IEvent{}, isCollide{ _isCollide }, rot{ _rot }, 
			speed{ _speed }, scale{ _scale }, pos{ _pos }, 
		    velocity{ _velocity }, texName{ _texName } {}
		bool isCollide;
		float rot,speed, scale;
		HTVector2* pos,*velocity;
		const char * texName;
	};

	struct EV_PLAY_SOUND : public IEvent
	{
		EV_PLAY_SOUND() :IEvent{} {}
	};

	struct EV_NEW_UI_ELEMENT : public IEvent
	{
		EV_NEW_UI_ELEMENT() {}
		EV_NEW_UI_ELEMENT(CanvasComponent* _canvas,
		HTVector2 _initialPosition,
		CanvasElementEnum _elementType,
		const char * _elementEntityName,
		const char * _uiElementSpriteName,
		const char * _uiTextLabel = "",
		const char * _uiHoverSpriteName = "",
		const char * _uiClickSpriteName = "",
		void(*_ButtonPressFunc)(void) = nullptr) 
			:IEvent{},
			canvas{ _canvas },
			initialPosition{ _initialPosition },
			elementType{_elementType},
			elementEntityName{ _elementEntityName },
			uiElementSpriteName{ _uiElementSpriteName },
			uiTextLabel{ _uiTextLabel },
			uiHoverSpriteName{ _uiHoverSpriteName },
			uiClickSpriteName{ _uiClickSpriteName },
			ButtonPressFunc{ _ButtonPressFunc }{}

		CanvasComponent* canvas;
		HTVector2 initialPosition;
		CanvasElementEnum elementType;
		const char * elementEntityName;
		const char * uiElementSpriteName;
		const char * uiTextLabel = "";
		const char * uiHoverSpriteName = "";
		const char * uiClickSpriteName = "";
		void(*ButtonPressFunc)(void) = nullptr;
	};

	struct EV_PLAYER_COLLISION_ON_ENTER : public IEvent
	{
		EV_PLAYER_COLLISION_ON_ENTER() {}
		EV_PLAYER_COLLISION_ON_ENTER(CollisionComponent* obj1, CollisionComponent* obj2)
			:IEvent{ }, object1{ obj1 }, object2{ obj2 }{}
		CollisionComponent* object1;
		CollisionComponent* object2;
	};

	struct EV_PLAYER_COLLISION_ON_EXIT : public IEvent
	{
		EV_PLAYER_COLLISION_ON_EXIT(CollisionComponent* obj1, CollisionComponent* obj2)
			:IEvent{ }, object1{ obj1 }, object2{ obj2 }{}
		EV_PLAYER_COLLISION_ON_EXIT() :IEvent{} {}
		CollisionComponent* object1;
		CollisionComponent* object2;
	};
}






class EventManager
{
protected : 
public:
	void Update();
	void ProcessEvents();

	template<typename T>
	bool AddListener(EventListener<T>* listener)
	{
		auto type = getTypeIndex<T>();
		auto v_Listener = m_l_ListenerList.find(type);

		if (v_Listener != m_l_ListenerList.end())
		{
			v_Listener->second.push_back(listener);
		}
		else
		{
			std::vector<BaseEventListener*> v_newEvent;
			v_newEvent.push_back(listener);
			m_l_ListenerList.insert({ type,v_newEvent });
		}
		return true;
	}

	template<typename T>
	void RemoveListener(EventListener<T>* listener)
	{
		auto type		= getTypeIndex<T>();
		auto v_Listener = m_l_ListenerList.find(type);
		for (auto i_Listener = v_Listener->second.begin();i_Listener <= v_Listener->second.end(); ++i_Listener)
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
				//EventQueue.push_back(static_cast<Events::IEvent>(event));
				Listener->Receive(event);
			}
			return true;
		}
		else
		{
			return false;
		}
	}


	EventManager(Logger* logger);
	virtual ~EventManager();
private:
	std::map<std::type_index,std::vector<BaseEventListener*>> m_l_ListenerList;
	Logger* m_o_Logger;
	std::list<Events::IEvent> EventQueue;
};

#endif