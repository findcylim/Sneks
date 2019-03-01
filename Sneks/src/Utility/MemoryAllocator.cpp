#include "MemoryAllocator.h"
#include <iostream>
#include <set>
#include <iomanip>

int allocCounter = 0;
int deleteCounter = 0;
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
