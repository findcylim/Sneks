/* Start Header****************************************************************/
/*!
\file Logger.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code for a logging system for any errors occuring.
Not used much for now.

\par Contribution : Adam - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef LOGGER_H
#define LOGGER_H
#pragma once
#include <fstream>
#include <ctime>

enum kLogNum
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
	void LogMessage(kLogNum lognum, const char * message, ...);
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