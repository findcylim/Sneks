#pragma once
#include <fstream>

class Logger
{

	//Variables
	std::ofstream LogFile;
public:
	Logger();
	bool LogMessage(const char * Message, ...);
	~Logger();

};

