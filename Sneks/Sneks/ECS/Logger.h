#pragma once
#include <fstream>

class Logger
{

	//Variables
	std::ofstream LogFile;
public:

	/*
		Log Message Function
		-Takes in a const char * string and logs it into Log.txt 
		 to monitor our system logs to improve debugging.
	*/
	bool LogMessage(const char * Message, ...);
	Logger();
	~Logger();

};

