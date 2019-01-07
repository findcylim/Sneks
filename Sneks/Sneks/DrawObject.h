#pragma once
#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H
#include "AEEngine.h"


class DrawObject {
protected:
	float		      m_fRotation = 0;
	float			  m_fVelocity = 0;
	float			  m_fPositionX;
	float			  m_fPositionY;
	float			  m_fSizeX;
	float			  m_fSizeY;
	AEGfxTexture*	  m_pTex;
	AEGfxVertexList*  m_pObj;

	
public:
	void SetRotation(float f);
	void SetVelocity(float f);
	float GetRotation();
	float GetVelocity();
	float GetPositionX();
	float GetPositionY();
	DrawObject(float posX, float posY, float sizeX, float sizeY, AEGfxTexture* tex);
	~DrawObject(void);
	void Draw();
	void Update();
	//void SetScale(float x, float y);

};

#endif