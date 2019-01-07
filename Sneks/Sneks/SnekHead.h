#pragma once
#include "DrawObject.h"
#include <Windows.h>

class SnekHead: public DrawObject {
private:
	AEGfxTexture* m_pSnekHed;
	AEGfxTexture* m_pSnekHedL;
	AEGfxTexture* m_pSnekHedR;
	AEGfxTexture* m_pSnekHedBoost;
	AEGfxTexture* m_pSnekHedSmoke;
	DrawObject* m_pParticles;

public:
	void Update();
	SnekHead(float posX, float posY, float sizeX, float sizeY, AEGfxTexture* tex, AEGfxTexture * snekHedL, AEGfxTexture * snekHedR);
	void SetParticles(AEGfxTexture*, AEGfxTexture*);
	void DrawParticles();
	void Draw();
};