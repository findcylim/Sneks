#include "Logger.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <stdarg.h>
#include <string>


Logger::Logger(const char * filename)
{
	LogFile.open(filename, std::ios::trunc);

	if (!LogFile.is_open())
	{
		std::cout << "ERROR : LOG FILE DID NOT OPEN" << std::endl;
		LogOpen = false;
	}
	else
		LogOpen = true;
}


void Logger::LogMessage(kLogNum,const char * Message, ...)
{
	std::string MessageBuffer;

	time_t l_LogTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	va_list args;
	va_start(args,Message);

	std::ofstream *LoggerFile = &LogFile;;

	localtime_s(&buf, &l_LogTime);

	MessageBuffer += '[' + std::to_string(buf.tm_mday) + '/' + std::to_string((buf.tm_mon + 1)) + '/' + std::to_string((buf.tm_year + 1900)) + ' ';
	std::setfill('0'); 
	std::setw(2);
	MessageBuffer += std::to_string(buf.tm_hour) + ':';
	std::setw(2);
	MessageBuffer += std::to_string(buf.tm_min)  + ']' + " : ";

	while (*Message != '\0')
	{
		if (*Message == '%')
		{
			Message++;
			switch (*Message)
			{
				case 'd':
				{
					MessageBuffer += std::to_string(va_arg(args, int));
					break;
				}
				case 's':
				{
					MessageBuffer += va_arg(args, char*);
					break;
				}
				case 'f':
				{
					double d = va_arg(args, double);
					double tempd = d;
					int size = 1;
					char temp = 0;

					while ((tempd / size) >= 1)
					{
						size *= 10;
						temp++;
					}

					std::setprecision(6 + temp);
					MessageBuffer += std::to_string(d);
					break;
				}
			}
			Message++;
		}
		if (*Message != 0)
			MessageBuffer += *Message;
		Message++;

	}
	MessageBuffer += '\n';

	std::cout << MessageBuffer.c_str();

	// File is open and ok to write
	if (LoggerFile->is_open())
	{
		*LoggerFile << MessageBuffer.c_str();
	}
}

Logger::~Logger()
{
	LogFile.close();
}
