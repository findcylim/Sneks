#ifndef FILE_IO_H
#define FILE_IO_H
#pragma once
#include <fstream>


namespace FileIO
{
	int ParseDataFromFile(const char* fileName)
	{
		std::ifstream inFile;
		inFile.open(fileName, std::ios::in);
		int value;
		inFile >> value;
		return value;
	}

	void AppendToFile(const char* fileName, const char* stringToWrite)
	{
		std::ofstream outFile(fileName, std::ios::app);
		//Asset if file is open
		if (outFile.is_open())
		{
			//Write string to file
			outFile << stringToWrite << std::endl;
		}
	}

}


#endif //FILE_IO_H

