/* Start Header ***************************************************************/
/*!
\file MemoryAllocator.cpp
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief 
This file contains the source for custom memory allocator for this engine which
tracks the new and delete calls, logging each new and delete, and warns the user
if memory leaks were detected if program ends normally.

\par Contribution (hours): CY - 5

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "MemoryAllocator.h"
#include <iostream>
#include <set>
#include <iomanip>

int allocCounter = 0;
int deleteCounter = 0;
/**
 * \brief 
 * 
 * \param pMemory 
 */
void operator delete(void* pMemory)noexcept
{
	HT_free(pMemory);
}

#ifdef new 
#undef new
void* operator new(size_t size, const char* file, int line)
{
	return ::HT_alloc(size, file, line);
}

void* operator new [](size_t size, const char* file, int line)
{
	return ::HT_alloc(size, file, line);
}
#define new new(__FILE__,__LINE__)
#endif
/**
 * \brief Goes through the vector and write the leaks to text log file. Log
 * folder must exist for file to be written.
 *
 */
bool LogMemoryLeaks()
{
	{
		if (allAllocations.empty())
		{
			std::cerr << "There were " << allocCounter << " allocations and no leaks." << std::endl;
			return false;
		}

		char fileName[100] ={ '\0' };
		char timeDate[100] ={ '\0' };

		strcpy_s(fileName, sizeof(fileName), "../Logs/memory_leak_log_");

		auto time = std::time(nullptr);

		struct tm timeInfo;

		localtime_s(&timeInfo, &time);

		strftime(timeDate, sizeof(timeDate), "%d%m%y %H.%M.%S", &timeInfo);

		strcat_s(fileName, sizeof(fileName), timeDate);
		strcat_s(fileName, sizeof(fileName), ".txt\0");

		std::ofstream outFile(fileName, std::ios::trunc);
		//Asset if file is open
		if (outFile.is_open())
		{
			outFile << ::allAllocations.size() << " MEMORY LEAK(S) DETECTED!" << std::endl;
			std::set<MallocDebugData> uniqueLeaks;
			for (auto pairing : allAllocations)
			{
				outFile << "At address (" << pairing.first << "), Declared in file " << pairing.second.file << ", line " << pairing.second.line << std::endl;
				uniqueLeaks.insert(pairing.second);
			}
			outFile << std::endl << std::endl << "Breakdown: " << std::endl << std::endl;
			outFile << "Unique leaks found: " << uniqueLeaks.size() << std::endl;
			for (auto unique : uniqueLeaks)
			{
				outFile << "File: " << std::left << std::setw(60) << unique.file << ", line " << unique.line << std::endl;
			}
		}
		::allAllocations.clear();
		outFile.close();
		
	}
	return true;
}
