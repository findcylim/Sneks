#include "Background.h"



Background::Background(int instancesX, int instancesY, AEGfxTexture* bgTexture)
{
	float screenSizeX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	float screenSizeY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	for (int i_Background = -instancesX; i_Background <= instancesX; i_Background++) {
		for (int i_BackgroundY = -instancesY; i_BackgroundY <= instancesY; i_BackgroundY++) {
			auto bgInstance = new DrawObject(i_Background * screenSizeX, i_BackgroundY * screenSizeY,
														screenSizeX, screenSizeY, bgTexture);
			m_BgInstances.push_back(bgInstance);
		}
	}
}


Background::~Background()
{
}

void Background::Draw()
{
	for (auto& i_Backgrounds : m_BgInstances) {
		i_Backgrounds->Draw();
	}
}
