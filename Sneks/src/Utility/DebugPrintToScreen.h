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

