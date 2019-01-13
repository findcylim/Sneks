#ifndef LOGGER_H
#define LOGGER_H
#pragma once
#include <fstream>
#include <ctime>

enum LogNum
{
	LOGGER_SYSTEM,
	LOGGER_EDITOR
};

class Logger
{
public:
	/*

		Takes in a message and logs it into the log.txt file.
		<This function will take care of formatting and time>

		Returns 0 if output to file ok
		Returns -1 if something went wrong with the Log file.
		I
		For now only use LOGGER_SYSTEM
		I
		LOGGER_SYSTEM,
		LOGGER_EDITOR

	*/
	void LogMessage(LogNum lognum, const char * message, ...);
	Logger(const char * filename);
	~Logger();
private:
	//Variables
	std::ofstream LogFile;
	bool LogOpen;
	char buffer[26];
	struct tm buf;
};

#endif