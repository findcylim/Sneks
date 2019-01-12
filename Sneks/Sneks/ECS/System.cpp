#include "System.h"
#include <iostream>


void System::Update()
{
	std::cout << "Error 2001 : Updating a non overriden system" << std::endl;
}


short System::GetID()
{
	return SystemID;
}
const char * System::GetName()
{
	return SystemName;
}
void System::SetID(short id)
{
	SystemID = id;
}
void System::SetName(const char* name)
{
	SystemName = name;
}
