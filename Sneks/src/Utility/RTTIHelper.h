#pragma once

#ifndef RTTI_HELPER
#define RTTI_HELPER

#include <typeindex>

template<typename T>
std::type_index getTypeIndex()
{
	return std::type_index(typeid(T));
}

#endif