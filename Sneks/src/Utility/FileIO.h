/* Start Header****************************************************************/
/*!
\file FileIO.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 18/03/2019
\date Modified: 26/03/2019
\brief This file contains helper functions for File IO, specifically to parse
data from PNG file and get its dimensions

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef FILE_IO_H
#define FILE_IO_H
#pragma once
#include "../Utility/MemoryAllocator.h"
#include <intrin.h>
#include <fstream>
#include <climits>

namespace FileIO
{
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

  /**
   * \brief Reads the PNG file and gets its dimensions.
   * 
   * \param retWidth stores the width here.
   * \param retHeight store the height here.
   */
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

	if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' &&
	  buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A &&
	  buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R')
	{
		*retWidth = (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
		*retHeight = (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);
	}
	inFile.close();*/
}


#endif //FILE_IO_H

