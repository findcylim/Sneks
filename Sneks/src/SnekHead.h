#ifndef SNEK_HEAD_H
#define SNEK_HEAD_H
#pragma once
#include "DrawObject.h"
#include <Windows.h>

class SnekHead : public DrawObject {
private:
	float				m_f_Boost;
	float				m_f_BoostGainRate = 0.05f;
	AEGfxTexture*  m_px_SnekHed;
	AEGfxTexture*  m_px_SnekHedL;
	AEGfxTexture*  m_px_SnekHedR;
	AEGfxTexture*  m_px_SnekHedBoost;
	AEGfxTexture*  m_px_SnekHedSmoke;
	DrawObject*	   m_px_Particles;
	unsigned short m_i_PlayerNumber        = 0;
	unsigned int   m_i_AccelerationKey     = VK_UP;
	unsigned int   m_i_BrakeKey            = VK_DOWN;
	unsigned int   m_i_LeftKey             = VK_LEFT;
	unsigned int   m_i_RightKey            = VK_RIGHT;
	unsigned int   m_i_BoostKey            = AEVK_RCTRL;
public:
	float GetBoost(void);
	void SetBoostGainRate(float);
	void Update();
	SnekHead(float posX, float posY, float sizeX, float sizeY, AEGfxTexture* tex);
	SnekHead(float posX, float posY, float sizeX, float sizeY, AEGfxTexture* tex, AEGfxTexture * snekHedL, AEGfxTexture * snekHedR);
	void SetParticles(AEGfxTexture*, AEGfxTexture*);
	void DrawParticles() const;
	void SetPlayer(int);
};
#endif //SNEK_HEAD_H