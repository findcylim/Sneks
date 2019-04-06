/* Start Header****************************************************************/
/*!
\file DebugPrintToScreen.h
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains helper functions to use AEGfxPrint.

\par Contribution : CY - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once
#include <AEMath.h>
#include <vector>

class DebugPrintToScreen
{
private:
	//static int       m_i_PosX;
	//static int       m_i_PosY;
	unsigned int     m_i_FontIndex;
	std::vector<s8*> m_x_PrintingList;
public:
	DebugPrintToScreen(const char* fontName, int fontSize);
	void PrintAtPosition(s8* toPrint, int posX, int posY);
	~DebugPrintToScreen();
	void Draw();
	void AddToPrintList(s8* toPrint);
};

