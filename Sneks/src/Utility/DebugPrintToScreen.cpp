/* Start Header ***************************************************************/
/*!
\file DebugPrintToScreen.cpp
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains helper functions to use AEGfxPrint.

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "DebugPrintToScreen.h"
#include <AEEngine.h>


DebugPrintToScreen::DebugPrintToScreen(const char* fontName = "Segoe UI",int fontSize = 20)
{
	//m_i_PosX = -900;
	//m_i_PosY = 450;
	m_i_FontIndex = AEGfxCreateFont(fontName, fontSize, false, false);

}

void DebugPrintToScreen::PrintAtPosition(s8* toPrint, int posX = 900, int posY = 450)
{
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxPrint(m_i_FontIndex, toPrint, posX, posY, 1, 0, 0);
}

DebugPrintToScreen::~DebugPrintToScreen()
{
}

void DebugPrintToScreen::Draw()
{
	for (unsigned i_PrintingList = 0; i_PrintingList < m_x_PrintingList.size(); i_PrintingList++)
	{
		PrintAtPosition(m_x_PrintingList.at(i_PrintingList), -900, 450 - i_PrintingList * 30);
	}
}

void DebugPrintToScreen::AddToPrintList(s8* toPrint)
{
	m_x_PrintingList.push_back(toPrint);
}