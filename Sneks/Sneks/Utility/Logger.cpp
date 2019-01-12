#include "Logger.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <stdarg.h>
#include <string>

Logger* Logger::instance = 0;
struct tm Logger::buf;
std::ofstream Logger::LogFile;



Logger::Logger()
{
	LogFile.open("log.txt", std::ios::trunc);

	if (!LogFile.is_open())
	{
		std::cout << "ERROR : LOG FILE DID NOT OPEN" << std::endl;
		LogOpen = false;
	}
	else
		LogOpen = true;
}


void Logger::LogMessage(LogNum LogNumber,const char * Message, ...)
{
	if (instance == 0)
	{
		instance = new Logger();
	}
	std::string MessageBuffer;

	time_t a_LogTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	va_list args;
	const char * FirstMessage = Message;
	va_start(args,Message);

	std::ofstream *LoggerFile;

	localtime_s(&buf, &a_LogTime);

	switch (LogNumber)
	{
		case LOGGER_SYSTEM:
			LoggerFile = &LogFile;
			break;
		case LOGGER_EDITOR:
			LoggerFile = &LogFile; //TODO Change this to an editor log file once we start developing an editor.
			break;
		default:
			LoggerFile = &LogFile;
			break;
	}

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
					std::fixed;
					break;
				}
				Message++;
			}

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


void Logger::DeleteLogger()
{
	delete(instance);
}

Logger::~Logger()
{
	LogFile.close();
}
