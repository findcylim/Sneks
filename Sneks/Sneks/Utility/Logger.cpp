#include "Logger.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <stdarg.h>





Logger::Logger()
{
	LogFile.open("log.txt", std::ios::app);

	if (!LogFile.is_open())
	{
		std::cout << "ERROR : LOG FILE DID NOT OPEN" << std::endl;
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
bool Logger::LogMessage(const char * Message, ...)
{
	time_t a_LogTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	va_list args;
	va_start(args,Message);

	localtime_s(&buf, &a_LogTime);

	// File is open and ok to write
	if (LogFile.is_open())
	{
		LogFile << "[" << buf.tm_mday << "/" << buf.tm_mon + 1 << "/" << buf.tm_year + 1900
			<< " " << std::setfill('0') << std::setw(2) << buf.tm_hour << ":" << std::setw(2)
			<< buf.tm_min << "]" << " : ";
		while (*Message != '\0') 
		{
			if (*Message == '%')
			{
				Message++;
				switch (*Message)
				{
					case 'd':
					{
						int i = va_arg(args, int);
						LogFile << i;
						break;
					}
					case 's':
					{
						char * c = va_arg(args, char*);
						LogFile << c;
						break;
					}
					case 'f': 
					{
						double d = va_arg(args, double);
						double tempd = d;
						int size = 1;
						std::streamsize p = LogFile.precision();
						char temp = 0;
						while ((tempd/size) >= 1)
						{
							size *= 10;
							temp++;
						}
						LogFile.precision(6 +temp);
						LogFile << d;
						LogFile.precision(p);
						break;
					}
				}
				Message++;
			}
			else if (*Message == '\\')
			{
				Message++;
				switch (*Message)
				{
				case 'n':
					{
						LogFile << std::endl;
						break;
					}
				}
			}
			if(*Message!=0)
				LogFile << *Message;
			Message++;
		}
		LogFile << std::endl;
		return true; // Output fine
	}
	return false; // Something went wrong
}



Logger::~Logger()
{
	LogFile.close();
}
