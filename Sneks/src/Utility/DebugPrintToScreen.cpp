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