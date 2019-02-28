#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H
#pragma once

#include <algorithm>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <ctime>
#include <fstream>

struct MallocDebugData
{
	const char* file;
	int line;
};

namespace {
	std::map<void*, MallocDebugData> allAllocations = {};

	void* HT_alloc(size_t size, const char* file, int line);

	void HT_free(void* pMemory);
}

void* operator new(size_t size, const char* file, int line);


void* operator new[](size_t size, const char* file, int line);


void operator delete(void* pMemory) noexcept;
 
void operator delete(void* pMemory, const char*, int) noexcept;

void operator delete[](void* pMemory, const char*, int) noexcept;

void LogMemoryLeaks();

#endif //MEMORY_ALLOCATOR_H