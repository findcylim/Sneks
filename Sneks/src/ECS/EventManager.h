
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

	struct EV_SNEK_INVULNERABLE
	{
		SnekHeadComponent* snekHead;
	};

	struct EV_CREATE_PROJECTILE
	{
		bool isCollide;
		float rot,speed, scale;
		HTVector2* pos,*velocity;
		const char * texName;
	};

	struct EV_PLAY_SOUND
	{

	};
}





/*
	<Standard function pointer format for callbacks>
	-Returns void.
	-Takes in void* to data structure, of which you will have to 
	typecast in your callback function.
	-Takes in a void* to member object of which the callback 
	came from.
*/
typedef void(*FunctionP)(void * data,void* callee);

typedef struct CallbackT
{
	FunctionP m_function;
	short m_EventId;
	void* m_CalleePtr;
}CallbackT;

typedef CallbackT* CallbackP;

template<typename T>
struct Event 
{	
	short EventId;
	T* Data;
	Event(short id,T* data) : EventId(id), Data(data) {}
};



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
		for (auto i_Listener = v_Listener->second.begin();i_Listener!=v_Listener->second.end();i_Listener++)
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
				Listener->receive(event);
			}
			return true;
		}
		else
		{
			return false;
		}
	}


	bool RemoveCallbackFromEvent(short EventID, FunctionP FPRef, void* callee);
	bool RemoveCallback(FunctionP FPRef, void* callee);

	void ResetInstance();
	void Initialize();
	EventManager(Logger* logger);
	virtual ~EventManager();
private:
	std::map<std::type_index,std::vector<BaseEventListener*>> m_l_ListenerList;
	bool hasEvent(short EventId);
	Logger* m_o_Logger;
	std::vector<std::vector<CallbackP>> m_EventCallBackList;
	//template<typename T>
	//bool AddListener(EventListener<T>* listener);
};

#endif