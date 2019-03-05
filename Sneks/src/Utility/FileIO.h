#ifndef FILE_IO_H
#define FILE_IO_H
#pragma once
#include "../Utility/MemoryAllocator.h"
#include <intrin.h>
#include <fstream>
#include <climits>

namespace FileIO
{
	/*
	template <typename T>
	T swap_endian(T u)
	{
		union
		{
			T u;
			unsigned char u8[sizeof(T)];
		} source, dest;

		source.u = u;

		for (size_t k = 0; k < sizeof(T); k++)
			dest.u8[k] = source.u8[sizeof(T) - k - 1];

		return dest.u;
	}*/

	inline int ParseDataFromFile(const char* fileName)
	{
		std::ifstream inFile;
		inFile.open(fileName, std::ios::in);
		int value;
		inFile >> value;
		inFile.close();
		return value;
	}

	inline void AppendToFile(const char* fileName, const char* stringToWrite)
	{
		std::ofstream outFile(fileName, std::ios::app);
		//Asset if file is open
		if (outFile.is_open())
		{
			//Write string to file
			outFile << stringToWrite << std::endl;
		}
		outFile.close();
	}

	inline void ReadPngDimensions(const char* fileName, int* retWidth, int* retHeight)
	{
		unsigned x, y;
		std::ifstream inFile;
		inFile.open(fileName, std::ios::in | std::ios::binary);
		if (inFile.is_open()) {
			inFile.seekg(16);
			inFile.read(reinterpret_cast<char*>(&x), 4);
			inFile.read(reinterpret_cast<char*>(&y), 4);
			x = _byteswap_ulong(x);
			y = _byteswap_ulong(y);
			*retWidth = x;
			*retHeight = y;
			inFile.close();
		}
	}

	/*		std::ifstream inFile;
	inFile.open(fileName, std::ios::binary);
	char* buf = new char[24];
	inFile.read(buf, 24);

	if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' && buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A
		&& buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R')
	{
		*retWidth = (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
		*retHeight = (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);
	}
	inFile.close();*/
}


#endif //FILE_IO_H

