#include "System.h"
#include <iostream>


void System::Update()
{
	std::cout << "Error 2001 : Updating a non overriden system" << std::endl;
}


short System::GetID()
{
	return m_s_SystemID;
}
const char * System::GetName()
{
	return m_c_SystemName;
}
void System::SetID(short iD)
{
	m_s_SystemID = iD;
}
void System::SetName(const char* name)
{
	m_c_SystemName = name;
}
