
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

	struct IEvent
	{
		IEvent() {}
		virtual ~IEvent() {}
	};

	//Declare all events here
	enum MoveKey
	{
		MOVE_KEY_LEFT = 0,
		MOVE_KEY_RIGHT,
		MOVE_KEY_UP,
		MOVE_KEY_DOWN
	};

	struct EV_PLAYER_MOVEMENT_KEY final : public IEvent
	{
		EV_PLAYER_MOVEMENT_KEY(PhysicsComponent* caller,MoveKey key) :IEvent{}
		{
			_caller = caller;
			_key	= key;
		}
		PhysicsComponent* _caller;
		MoveKey _key;
	};

	struct EV_PLAYER_COLLISION final : public IEvent
	{
		EV_PLAYER_COLLISION(CollisionComponent* object1, CollisionComponent* object2) :IEvent{}
		{
			_object1 = object1;
			_object2 = object2;
		}
		CollisionComponent* _object1;
		CollisionComponent* _object2;
	};

	struct EV_ENTITY_POOL_CHANGED final : public IEvent
	{
		EV_ENTITY_POOL_CHANGED() :IEvent{}
		{

		}
	};

	struct EV_SNEK_INVULNERABLE : public IEvent
	{
		EV_SNEK_INVULNERABLE(SnekHeadComponent* snekHead) :IEvent{}
		{
			_snekHead = snekHead;
		}
		SnekHeadComponent* _snekHead;
	};

	struct EV_CREATE_PROJECTILE : public IEvent
	{
		EV_CREATE_PROJECTILE(bool isCollide,
							float rot, float  speed, float  scale,
							HTVector2* pos, HTVector2 *velocity,
							const char * texName) 
							:IEvent{}
		{

			_isCollide = isCollide;
			_rot = rot;
			_speed = speed;
			_scale = scale;
			_pos = pos; 
			_velocity = velocity;
			_texName = texName;
		}
		bool _isCollide;
		float _rot, _speed, _scale;
		HTVector2* _pos, *_velocity;
		const char * _texName;
	};

	struct EV_PLAY_SOUND : public IEvent
	{
		EV_PLAY_SOUND() :IEvent{}
		{

		}
	};

	struct EV_NEW_UI_ELEMENT : public IEvent
	{
		EV_NEW_UI_ELEMENT(CanvasComponent* canvas,
		HTVector2 initialPosition,
		CanvasElementEnum elementType,
		const char * elementEntityName,
		const char * uiElementSpriteName,
		const char * uiTextLabel = "",
		const char * uiHoverSpriteName = "",
		const char * uiClickSpriteName = "",
		void(*ButtonPressFunc)(void) = nullptr) :IEvent{}
		{
			_canvas = canvas;
			_initialPosition = initialPosition;
			_elementType = elementType;
			_elementEntityName = elementEntityName;
			_uiElementSpriteName = uiElementSpriteName;
			_uiTextLabel = uiTextLabel;
			_uiHoverSpriteName = uiHoverSpriteName;
			_uiClickSpriteName = uiClickSpriteName;
			_ButtonPressFunc = ButtonPressFunc;
		}
		CanvasComponent* _canvas;
		HTVector2 _initialPosition;
		CanvasElementEnum _elementType;
		const char * _elementEntityName;
		const char * _uiElementSpriteName;
		const char * _uiTextLabel = "";
		const char * _uiHoverSpriteName = "";
		const char * _uiClickSpriteName = "";
		void(*_ButtonPressFunc)(void) = nullptr;
	};

	struct EV_PLAYER_COLLISION_ON_ENTER : public IEvent
	{
		EV_PLAYER_COLLISION_ON_ENTER(CollisionComponent* object1, CollisionComponent* object2) :IEvent{}
		{
			_object1 = object1;
			_object2 = object2;
		}
		CollisionComponent* _object1;
		CollisionComponent* _object2;
	};

	struct EV_PLAYER_COLLISION_ON_EXIT : public IEvent
	{
		EV_PLAYER_COLLISION_ON_EXIT(CollisionComponent* object1, CollisionComponent* object2) :IEvent{}
		{
			_object1 = object1;
			_object2 = object2;
		}
		CollisionComponent* _object1;
		CollisionComponent* _object2;
	};

	struct EV_MOUSE_ONCLICK : public IEvent
	{
		EV_MOUSE_ONCLICK() :IEvent{}
		{

		}

	};
}

class EventManager
{
protected:
public:
	void Update();
	void ProcessEvents()
	{
		for (auto eventData : m_l_EventQueue)
		{
			for (auto listner : m_l_ListenerList)
			{
				if()
			}
			if (i_Listener != m_l_ListenerList.end())
			{
				for (auto* baseListener : i_Listener->second)
				{

				}
			}

		}
	}


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
			auto ii_Listener = static_cast<BaseEventListener*>(listener);
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
	bool EmitEvent(const T& eventData)
	{
		auto i_Listener = m_l_ListenerList.find(getTypeIndex<T>());
		if  (i_Listener !=m_l_ListenerList.end())
		{
			for (auto* baseListener : i_Listener->second)
			{
				auto* Listener = reinterpret_cast<EventListener<T>*>(baseListener);
				if (Listener->systemListener->m_b_isActive)
				{
					T* eventDataCopy = new T(eventData);
					m_l_EventQueue.push_back(static_cast<Events::IEvent*>(eventDataCopy);
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename T>
	T* ReturnEventData()
	{
		for (auto& eventData : m_l_EventQueue)
		{
			if (T* p = dynamic_cast<T*>(eventData))
			{
				return static_cast<T*>(eventData);
			}
		}
		return nullptr;
	}
	void ProcessEvents();
	EventManager();
	virtual ~EventManager();
private:
	std::map<std::type_index, std::vector<BaseEventListener*>> m_l_ListenerList;
	std::list<Events::IEvent*> m_l_EventQueue;
};

#endif