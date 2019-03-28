/* Start Header ***************************************************************/
/*!
\file MemoryAllocator.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief 
This file is the header for custom memory allocator for this engine    
which tracks the new and delete calls, logging each new and delete, and warns
the user if memory leaks were detected if program ends normally.

\par Contribution (hours): CY - 5

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

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
/**
 * \brief class to hold data for logging
 * 
 */
struct MallocDebugData
{
	const char* file;
	int line;
	bool operator== (const MallocDebugData& rhs) const
	{
		return file == rhs.file && line == rhs.line;
	}

};
/**
 * \brief operator std::less to support usage of sets
 * 
 * \param lhs 
 * \param rhs 
 * \return true 
 * \return false 
 */
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

/**
 * \brief Internal namespace to prevent outside usage
 * 
 */
namespace 
{
	std::vector<void*> allAlloc2;
	std::map<void*, MallocDebugData> allAllocations;
  /**
   * \brief Implementation of new to store file/line
   * 
   */
	void* HT_alloc(size_t size, const char* file, int line)
	{
		static int counter{};
		++allocCounter;
		void* allocation = malloc(size);
		allAllocations.insert (std::make_pair(allocation, MallocDebugData{ file,line } ));
		allAlloc2.push_back(allocation);
		return allocation;
	}

  /**
   * \brief Implementation of free, removes from allocated vector
   *
   */
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

/**
 * \brief delegating new that calls HT_alloc 
 */
void* operator new(size_t size, const char* file, int line);

/**
 * \brief delegating new that calls HT_alloc 
 */
void* operator new[](size_t size, const char* file, int line);

/**
 * \brief delegating delete that calls HT_free
 */
inline void operator delete(void* pMemory, const char*, int) noexcept
{
	operator delete(pMemory);
}

/**
 * \brief delegating delete that calls HT_free
 */
inline void operator delete[](void* pMemory, const char*, int)noexcept
{
	operator delete(pMemory);
}

/**
 * \brief delegating delete that calls HT_free
 */
void operator delete(void* pMemory)noexcept;

#define new new(__FILE__,__LINE__)

/**
 * \brief Checks the current vector for allocations and asserts if there are
 *        leaks. Should be called at end of program
 * 
 */
bool LogMemoryLeaks();

#endif //MEMORY_ALLOCATOR_H