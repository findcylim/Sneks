#include "System.h"
#include <iostream>


void BaseSystem::Update()
{
	std::cout << "Error 2001 : Updating a non overriden system" << std::endl;
}


short BaseSystem::GetID()
{
	return m_s_SystemID;
}
const char * BaseSystem::GetName()
{
	return m_c_SystemName;
}
void BaseSystem::SetID(short iD)
{
	m_s_SystemID = iD;
}
void BaseSystem::SetName(const char* name)
{
	m_c_SystemName = name;
}
