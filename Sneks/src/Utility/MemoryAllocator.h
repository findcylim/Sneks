#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H
#pragma once

#include <algorithm>
#include <cstdlib>
#include <vector>
#include <map>
#include <chrono>
#include <ctime>
#include <fstream>

struct MallocDebugData
{
	const char* file;
	int line;
	bool operator== (const MallocDebugData& rhs) const
	{
		return file == rhs.file && line == rhs.line;
	}

};

inline bool operator <(const MallocDebugData& lhs, const MallocDebugData& rhs)
{
	if (lhs.file == rhs.file)
	{
		return lhs.line < rhs.line;
	}
	return lhs.file < rhs.file;
}


extern int allocCounter;
extern int deleteCounter;

namespace {
	std::vector<void*> allAlloc2;
	std::map<void*, MallocDebugData> allAllocations;
	void* HT_alloc(size_t size, const char* file, int line)
	{
		static int counter{};
		++allocCounter;
		void* allocation = malloc(size);
		allAllocations.insert (std::make_pair(allocation, MallocDebugData{ file,line } ));
		allAlloc2.push_back(allocation);
		return allocation;
	}

	void HT_free(void* pMemory)
	{
		free(pMemory);
		if (allAllocations.empty())
			return;

		auto key = ::allAllocations.find(pMemory);
		if (key != allAllocations.end())
			::allAllocations.erase(key);
	}
}

void* operator new(size_t size, const char* file, int line);

void* operator new[](size_t size, const char* file, int line);

inline void operator delete(void* pMemory, const char*, int) noexcept
{
	operator delete(pMemory);
}

inline void operator delete[](void* pMemory, const char*, int)noexcept
{
	operator delete(pMemory);
}

void operator delete(void* pMemory)noexcept;

#define new new(__FILE__,__LINE__)
//#define malloc "Use new/delete instead of malloc/free!"
//#define free "Use new/delete instead of malloc/free!"
//
//namespace{
//	std::map<void*, MallocDebugData> allAllocations;
//
//	void* HT_alloc(size_t size, const char* file, int line);
//
//	void HT_free(void* pMemory);
//}
//void* operator new (size_t size);
//void* operator new(size_t size, const char* file, int line);
//
//
//void* operator new[](size_t size, const char* file, int line);
//
//
//void operator delete(void* pMemory) noexcept;
// 
//void operator delete(void* pMemory, const char*, int) noexcept;
//
//void operator delete[](void* pMemory, const char*, int) noexcept;
//
bool LogMemoryLeaks();

#endif //MEMORY_ALLOCATOR_H