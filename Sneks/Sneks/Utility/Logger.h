#pragma once
#include <fstream>
#include <ctime>

class Logger
{

public:
	/*
		<Log Message Function>
		-Takes in a const char * string and logs it into Log.txt .
		-Has some conversion specifiers, %s, %d, %f(only 5 decimal point precision).
	*/
	bool LogMessage(const char * Message, ...);
	
	Logger();
	~Logger();
private:
	//Variables
	std::ofstream LogFile;
	bool LogOpen;
	char buffer[26];
	struct tm buf;
};

