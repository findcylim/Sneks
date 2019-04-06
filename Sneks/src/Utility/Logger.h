/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

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