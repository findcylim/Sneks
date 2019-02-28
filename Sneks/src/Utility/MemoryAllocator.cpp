#include "MemoryAllocator.h"


namespace {
	void* HT_alloc(size_t size, const char* file, int line)
	{
		static int counter{};
		void* allocation = malloc(size);
		allAllocations[allocation] = MallocDebugData{ file,line };
		return allocation;
	}

	void HT_free(void* pMemory)
	{
		auto key = ::allAllocations.find(pMemory);
		free(pMemory);
		if (key != allAllocations.end())
			::allAllocations.erase(key);
	}
}

void* operator new(size_t size, const char* file, int line)
{
	return ::HT_alloc(size, file, line);
}

void* operator new [](size_t size, const char* file, int line)
{
	return ::HT_alloc(size, file, line);
}

void operator delete(void* pMemory, const char*, int)
{
	operator delete(pMemory);
}

void operator delete[](void* pMemory, const char*, int)
{
	operator delete(pMemory);
}

void operator delete(void* pMemory)
{
	HT_free(pMemory);
}

void LogMemoryLeaks()
{
	{
		if (allAllocations.empty())
		{
			return;
		}

		char fileName[100] ={ '\0' };
		char timeDate[100] ={ '\0' };

		strcpy_s(fileName, sizeof(fileName), "./Logs/memory_leak_log_");

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
			outFile << ::allAllocations.size() << " MEMORY LEAK DETECTED!" << std::endl;
			for (auto pairing : allAllocations)
			{
				outFile << "At address (" << pairing.first << "), Declared in file " << pairing.second.file << ", line " << pairing.second.line << std::endl;
			}
			outFile << std::endl << std::endl << "Breakdown: " << std::endl << std::endl;
		}
		::allAllocations.clear();
		outFile.close();

	}
}
