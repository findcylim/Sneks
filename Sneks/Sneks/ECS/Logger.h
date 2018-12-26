#pragma once
#include <fstream>

class Logger
{

	//Variables
	std::ofstream LogFile;
public:
	Logger();
	char LogMessage(const char * Message);
	~Logger();

};

