#pragma once
#include <fstream>

class Logger
{

public:
	//Variables
	static std::ofstream LogFile;

	/*
		Log Message Function
		-Takes in a const char * string and logs it into Log.txt 
		 to monitor our system logs to improve debugging.
	*/
	static bool LogMessage(const char * Message, ...);
	//MAKE A DESTROY FOR THE SINGLETON LOGGER
	static Logger * Instance();

	static Logger* instance;
	Logger();
	~Logger();
};

