#ifndef SYSTEM_H
#define SYSTEM_H

#pragma once

//Virtual System Class
//To be overriden by actual systems
//Needs to be used to manage different systems in the SystemManager
class System
{
protected:
	short		m_s_SystemID;
	const char* m_c_SystemName;
	bool		m_b_isActive;
public:
	virtual void Update();
	short GetID();
	const char * GetName();
	void SetID(short id);
	void SetName(const char* name);
};

#endif