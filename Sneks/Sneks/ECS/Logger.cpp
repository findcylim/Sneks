#include "Logger.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
using namespace std;

bool LogOpen;
char buffer[26];
struct tm buf;

Logger::Logger()
{
	LogFile.open("log.txt", ios::app);

	if (!LogFile.is_open())
	{
		cout << "ERROR : LOG FILE DID NOT OPEN" << endl;
		LogOpen = false;
	}
	else
		LogOpen = true;
}

/***************************************************************************************

	Takes in a message and logs it into the log.txt file.
	<This function will take care of formatting and time>

	Returns 0 if output to file ok
	Returns -1 if something went wrong with the Log file.

***************************************************************************************/
char Logger::LogMessage(const char * Message)
{
	time_t a_LogTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	localtime_s(&buf, &a_LogTime);
	
	// File is open and ok to write
	if (LogFile.is_open())
	{
		LogFile << "["		   << buf.tm_mday  << "/"     << buf.tm_mon + 1 << "/" << buf.tm_year + 1900 
			    << " "         << setfill('0') << setw(2) << buf.tm_hour    << ":" << setw(2)     
			    << buf.tm_min  << "]"          << " : "   << Message        << endl;

		return 0; // Output fine
	}

	return -1; // Something went wrong
}



Logger::~Logger()
{
	LogFile.close();
}
